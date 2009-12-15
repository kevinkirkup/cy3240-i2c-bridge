/* Copyright (c) 2007 - 2009, Christian Hujer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the AceUnit developers nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package net.sf.aceunit;

import java.io.*;
import java.util.*;

import net.sf.japi.io.args.ArgParser;
import net.sf.japi.io.args.BasicCommand;
import net.sf.japi.io.args.Option;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

/**
 * Program for generating the code that's needed to execute a fixture.
 * This is a prototype in Java.
 * The final version should be in C so a developer is not required to run Java in a C development environment.
 *
 * @author <a href="mailto:cher@riedquat.de">Christian Hujer</a>
 */
public class GenTest extends BasicCommand {

    /**
     * Main program.
     *
     * @param args Currently the first argument must be the base name of the file to create a fixture for, e.g. "example", without file ending.
     * @throws IOException in case of I/O problems.
     * @note the interface is subject of change.
     */
    public static void main(@NotNull final String... args) throws IOException {
        ArgParser.simpleParseAndRun(new GenTest(), args);
    }

    /** Whether to overwrite write-protected files. */
    private boolean force;

    /** Whether or not to generate suites. */
    private boolean genSuites = true;

    /** What to print. */
    private final EnumSet<Print> printSet = EnumSet.noneOf(Print.class);

    /** Filename to which a table of all tests is written. */
    @Nullable private String allTestsFilename;

    /**
     * The IdGenerator which is used for generating unique ids for Suites.
     * It is used for all {@link Suite}s ({@link Pckg}s and {@link Fixture}s).
     */
    @NotNull
    private final IdGenerator idGenerator = new IdGenerator();

    /** The flattened list of all tests. */
    private final List<String> allTests = new ArrayList<String>();

    /** The root packages which were found so far. */
    private final List<Pckg> roots = new ArrayList<Pckg>();

    /**
     * Creates a GenTest instance.
     *
     * @throws IOException in case of I/O problems.
     */
    public GenTest() throws IOException {
    }

    /**
     * Sets whether overwriting of existing files should be enforced or not.
     *
     * @param force Whether overwriting of existing files should be enforced.
     */
    @Option({"f", "force"})
    public void setForce(@NotNull final Boolean force) {
        this.force = force;
    }

    /**
     * Sets which names to print.
     * @param print Which filenames to print.
     */
    @Option({"print"})
    public void setPrint(@NotNull final String print) {
        printSet.clear();
        for (final String element : print.split(",")) {
            printSet.add(Print.valueOf(element));
        }
    }

    /**
     * Disables suite generation.
     * If suite generation is turned of, the feature ACEUNIT_SUITES cannot be used.
     */
    @Option({"disableGenSuites"})
    public void disableGenSuites() {
        this.genSuites = false;
    }

    /**
     * Sets the filename of the file to which a table of all tests is written.
     * @param allTestsFilename Filename of the file to which a table of all tests shall be written.
     */
    @Option({"o"})
    public void setAllTestsFilename(@Nullable final String allTestsFilename) {
        this.allTestsFilename = allTestsFilename;
    }

    /**
     * Performs the generation of tests.
     *
     * @param basename Base name to generate tests for.
     *                 This may be a file when appending '.c' or a directory or both.
     *                 If it is neither nor, an {@link IllegalArgumentException} is thrown.
     * @return <code>true</code> if tests were found, otherwise <code>false</code>.
     * @throws IOException              In case of I/O problems.
     * @throws IllegalArgumentException in case <var>basename</var> is not a directory and no file <code><var>basename</var>.c</code> was found.
     */
    private boolean perform(@NotNull final String basename) throws IOException {
        final File baseDir = new File(basename);
        final File[] fixtureFiles = { new File(basename + ".c"), new File(basename + ".cpp") };
        boolean baseFound = false;
        boolean testsFound = false;
        if (baseDir.isDirectory()) {
            baseFound = true;
            final boolean testsFoundInDirectory = performPckg(baseDir, null, baseDir);
            testsFound = testsFoundInDirectory;
            if (!testsFoundInDirectory) {
                System.err.println(baseDir + ": warning: No test cases found. Maybe the annotations are missing?");
            }
        }
        for (final File fixtureFile : fixtureFiles) {
            if (fixtureFile.isFile()) {
                baseFound = true;
                final boolean testsFoundInFile = performFixture(fixtureFile.getParentFile(), new Pckg("dummy"), fixtureFile);
                testsFound |= testsFoundInFile;
                if (!testsFoundInFile) {
                    System.err.println(fixtureFile + ": warning: No test cases found. Maybe the annotations are missing?");
                }
            }
        }
        if (!baseFound) {
            System.err.println("AceUnit: " + basename + ": error: No such directory.");
            System.err.println("AceUnit: " + basename + ".c: error: No such file.");
            System.err.println("AceUnit: " + basename + ".cpp: error: No such file.");
            System.err.println("AceUnit: no input files");
        }
        if (!testsFound) {
            System.err.println("AceUnit: no test cases in " + basename);
        }
        return baseFound || testsFound;
    }

    /**
     * Performs the generation of tests for a package.
     *
     * @param base    Base directory of the tree, required to determine what part of the path is package.
     * @param parent  Parent package of this package, <code>null</code> if toplevel.
     * @param pckgDir Directory to treat as package.
     * @return <code>true</code> if the package tree contained a fixture, otherwise <code>false</code>
     * @throws IOException In case of I/O problems.
     */
    private boolean performPckg(@NotNull final File base, @Nullable final Pckg parent, @NotNull final File pckgDir) throws IOException {
        assert pckgDir.isDirectory();
        boolean containedFixture = false;
        final Pckg pckg = new Pckg("dummy");
        // XXX:2009-08-16:christianhujer:Hack to keep the first suite id "1".
        //     The correct solution would be to separate scan and output.
        //     However, that solution might require more memory.
        //     So before that solution is implemented, a footprint analysis is required.
        if (parent == null) {
            pckg.setId(idGenerator);
            roots.add(pckg);
        }
        for (final File dir : pckgDir.listFiles(SourceFiles.DIR_FILTER)) {
            containedFixture |= performPckg(base, pckg, dir);
        }
        for (final File fixtureFile : pckgDir.listFiles(SourceFiles.C_SOURCE_FILTER)) {
            containedFixture |= performFixture(base, pckg, fixtureFile);
        }
        for (final File fixtureFile : pckgDir.listFiles(SourceFiles.CPP_SOURCE_FILTER)) {
            containedFixture |= performFixture(base, pckg, fixtureFile);
        }
        if (containedFixture) {
            // XXX:2009-08-16:christianhujer:The id is already set above if there is no parent.
            if (parent != null) {
                pckg.setId(idGenerator);
            }
            final File cFile = new File(pckgDir, "Suite" + pckg.getId() + ".c");
            if (printSet.contains(Print.sources) || printSet.contains(Print.suites) || printSet.contains(Print.generated)) {
                System.out.println(cFile);
            }
            final String hSource = pckg.getCode("foo");
            if (genSuites) {
                SourceFiles.writeIfChanged(cFile, hSource, force);
            }
            allTests.add(pckgDir.toString().replaceAll("^\\./", "").replaceAll("[\\/]", "."));
        }
        if (containedFixture && parent != null) {
            parent.addSuite(pckg);
        }
        return containedFixture;
    }

    /**
     * Performs the generation of tests for a fixtureFile.
     *
     * @param base        Base directory of the tree, required to determine what part of the path is package.
     * @param pckg        Package to which the fixture will be added.
     * @param fixtureFile File that contains the fixtureFile.
     * @return <code>true</code> if the source code contained a fixture, otherwise <code>false</code>
     * @throws IOException In case of I/O problems.
     */
    private boolean performFixture(@NotNull final File base, @NotNull final Pckg pckg, @NotNull final File fixtureFile) throws IOException {
        if (fixtureFile.getName().matches("Suite\\d+\\.c")) {
            return false;
        }
        final String fixtureName = fixtureFile.getName().replaceAll("\\.c(pp)?$", "");
        final Fixture fixture = new Fixture(fixtureFile);
        final boolean containedFixture = fixture.containsTests();
        if (containedFixture) {
            fixture.createTestCases();
            for (final TestCase testCase : fixture.getTests()) {
                allTests.add(testCase.getName());
            }
            final File hFile = new File(fixtureFile.getParent(), fixtureName + ".h");
            if (printSet.contains(Print.sources) || printSet.contains(Print.fixtures)) {
                System.out.println(fixtureFile);
            }
            if (printSet.contains(Print.headers) || printSet.contains(Print.generated)) {
                System.out.println(hFile);
            }
            fixture.setId(idGenerator);
            final String hSource = fixture.getCode(fixtureName);
            SourceFiles.writeIfChanged(hFile, hSource, force);
            pckg.addSuite(fixture);
            allTests.add(fixtureName.replaceAll("[\\/]", "."));
        }
        return containedFixture;
    }

    /** Writes all tests to the file in {@link #allTestsFilename} if it is not null.
     * @throws IOException in case of I/O problems.
     */
    private void writeAllTests() throws IOException {
        final String filename = allTestsFilename;
        if (filename == null) {
            return;
        }
        final PrintWriter out = new PrintWriter(new OutputStreamWriter(new FileOutputStream(filename)));
        if (printSet.contains(Print.generated)) {
            System.out.println(allTestsFilename);
        }
        try {
            for (final Pckg pckg : roots) {
                writeAllTests(out, pckg);
            }
            // TODO:2009-08-16:christianhujer:Create list of test / suite ids.
            //for (final Integer id : new TreeSet<Integer>(allTests.keySet())) {
            //    out.format("%d: %s%n", id, allTests.get(id));
            //}
        } finally {
            out.close();
        }
    }

    /** Writes all tests of <var>test</var> including <var>test</var> itself to <var>out</var>.
     * @param out PrintWriter to append to.
     * @param test Package to append.
     * @throws IOException in case of I/O problems.
     */
    private void writeAllTests(@NotNull final PrintWriter out, @NotNull final Test test) throws IOException {
        out.format("%d: %s%n", test.getId(), test.getName());
        if (test instanceof Suite) {
            for (final Test subtest : ((Suite<Test>) test)) {
                writeAllTests(out, subtest);
            }
        }
    }

    /** {@inheritDoc} */
    public int run(@NotNull final List<String> args) throws Exception {
        if (args.isEmpty()) {
            help();
            return 1;
        }
        boolean okay = true;
        for (final String basename : args) {
            okay &= perform(basename);
        }
        writeAllTests();
        if (!okay) {
            System.err.println("AceUnit: warning: No test cases found.");
        }
        return okay ? 0 : 1;
    }

    /** Enum that describes what to print.
     * @author <a href="mailto:cher@riedquat.de">Christian Hujer</a>
     */
    static enum Print {

        /** Print suite names. */
        suites,

        /** Print fixture sources. */
        fixtures,

        /** Print generated files. */
        generated,

        /** Print header files. */
        headers,

        /** Print source files. */
        sources,
    }
} // class GenTest
