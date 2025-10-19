#!/bin/bash

function LongHash
{
	git rev-parse HEAD 2> /dev/null | sed "s/\(.*\)/\1/"
}

function ShortHash
{
	git rev-parse --short HEAD 2> /dev/null | sed "s/\(.*\)/\1/"
}

function CreateRevisionFile {
	local TIMESTAMP=$(date "+%d/%m/%Y @ %H:%M:%S")
	local LREVISION=$(echo "$(LongHash)" | tr '[:lower:]' '[:upper:]')
	local SREVISION=$(echo "$(ShortHash)" | tr '[:lower:]' '[:upper:]')
	local MAJOR_VERSION=255
	local MINOR_VERSION=255
	local BUGFIX_VERSION=255
	
	local OFILE="../Middlewares/Version/Version.h"
	
	# Create and write to the output file
	{
		echo "#ifndef __VERSION_H__"
		echo "#define __VERSION_H__"
		echo "#define   MAJOR_VERSION      \"$MAJOR_VERSION\""
		echo "#define   MINOR_VERSION      \"$MINOR_VERSION\""
		echo "#define   BUGFIX_VERSION     \"$BUGFIX_VERSION\""
		echo "#define   VERSION     	     \"$MAJOR_VERSION.$MINOR_VERSION.$BUGFIX_VERSION\""
		echo "#define   LREVISION          \"$LREVISION\""
		echo "#define   SREVISION          \"$SREVISION\""
		echo "#define   BUILDTIME         \"$TIMESTAMP\""
		echo "#endif  // __VERSION_H__"
	} > "$OFILE"
	
	echo "Version header file generated: $OFILE"
}





function main
{
	CreateRevisionFile
	echo " "
}

if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
	main ${@}
fi