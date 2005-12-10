#!/bin/sh

set -e

ACLOCAL=${ACLOCAL:-aclocal}
AUTOMAKE=${AUTOMAKE:-automake}
AUTOHEADER=${AUTOHEADER:-autoheader}
AUTOCONF=${AUTOCONF:-autoconf}

echo "*** You'll need the following packages to build jMList from repository:"
echo "***   - automake 1.8 or better"
echo "***   - autoconf 2.59 or better"

echo ""
echo "*** Running \`${ACLOCAL} -I m4'..."
${ACLOCAL} -I m4

echo ""
echo "*** Running \`${AUTOHEADER}'..."
${AUTOHEADER}

echo ""
echo "*** Running \`${AUTOMAKE} --gnu --add-missing'..."
${AUTOMAKE} --gnu --add-missing

echo ""
echo "*** Running \`${AUTOCONF}'..."
${AUTOCONF}

echo ""
echo "*** Development support files created. Now run \`./configure'."
echo "*** Suggestion: \`./configure --enable-maintainer-mode'. To see which other"
echo "    options are available, try \`./configure --help'."
