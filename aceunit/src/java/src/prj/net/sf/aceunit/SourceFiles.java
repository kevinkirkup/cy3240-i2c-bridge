/* Copyright (c) 2007 - 2008, Christian Hujer
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

import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;
import java.io.Reader;
import java.io.FileReader;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.FileFilter;
import java.io.BufferedReader;
import static java.util.logging.Logger.getAnonymousLogger;

/**
 * Utility class with methods for reading source files.
 *
 * @author <a href="mailto:cher@riedquat.de">Christian Hujer</a>
 */
public final class SourceFiles {

    /** FileFilter to get directories. */
    public static final FileFilter DIR_FILTER = new FileFilter() {
        /** {@inheritDoc} */
        public boolean accept(@NotNull final File pathname) {
            return pathname.isDirectory();
        }
    };

    /** FileFilter to get C source files. */
    public static final FileFilter C_SOURCE_FILTER = new FileFilter() {
        /** {@inheritDoc} */
        public boolean accept(final File pathname) {
            return pathname.isFile() && pathname.getName().endsWith(".c");
        }
    };

    /** FileFilter to get C++ source files. */
    public static final FileFilter CPP_SOURCE_FILTER = new FileFilter() {
        /** {@inheritDoc} */
        public boolean accept(final File pathname) {
            return pathname.isFile() && pathname.getName().endsWith(".cpp");
        }
    };

    /**
     * Flag to remember whether we've already warned the user about 1.6 being required for removing a write protection from a file.
     * We do not want to spam the user with the same message over and over again.
     */
    private static boolean warned16RequiredForSetWritable;

    /** Utility class - do not instanciate. */
    private SourceFiles() {
    }

    /**
     * Reads a source file.
     *
     * @param file File of the source to read.
     * @return String with the contents of the specified file.
     * @throws IOException in case of I/O problems.
     */
    public static String readSource(@NotNull final File file) throws IOException {
        final Reader in = new FileReader(file);
        final StringBuilder sb = new StringBuilder();
        final char[] buf = new char[4096];
        for (int charsRead; (charsRead = in.read(buf)) != -1;) {
            sb.append(buf, 0, charsRead);
        }
        in.close();
        return sb.toString();
    }

    /**
     * Reads a source file without comments.
     *
     * @param file File of the source to read.
     * @return String with the contents of the specified file.
     * @throws IOException in case of I/O problems.
     */
    public static String readSourceWithoutComments(@NotNull final File file) throws IOException {
        final Reader in = new CommentToWhitespaceReader(new BufferedReader(new FileReader(file)));
        final StringBuilder sb = new StringBuilder();
        final char[] buf = new char[4096];
        for (int charsRead; (charsRead = in.read(buf)) != -1;) {
            sb.append(buf, 0, charsRead);
        }
        in.close();
        return sb.toString();
    }

    /**
     * Writes a String to a file if the String differs from the file's content.
     *
     * @param file  File to write to.
     * @param text  String to write to the file.
     * @param force Set this to true if a write protection from the file should be removed.
     * @throws IOException In case of I/O problems.
     */
    public static void writeIfChanged(@NotNull final File file, @NotNull final String text, final boolean force) throws IOException {
        if (!file.exists() || !text.equals(readSource(file))) {
            if (force) {
                if (!file.canWrite()) {
                    getAnonymousLogger().info("Removing write protection from " + file);
                    try {
                        file.setWritable(true);
                    } catch (final NoSuchMethodError e) {
                        if (!warned16RequiredForSetWritable) {
                            getAnonymousLogger().warning("Java 1.6 required for removing write protections.");
                            warned16RequiredForSetWritable = true;
                        }
                    }
                }
            }
            final PrintWriter out = new PrintWriter(new FileWriter(file));
            try {
                out.append(text);
            } finally {
                out.close();
            }
        }
    }

} // class SourceReading
