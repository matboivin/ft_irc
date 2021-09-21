assert_exec()
{
	#$1 - command to run
	#$2 - expected standard output
	#$3 - expected standard error
	#$4 - expected exit code
	#Check that the executable as the expected output and exit code
	
	ERR_FILE="$0".err
	OUT_FILE="$0".out
	TO_CLEAN="$ERR_FILE $OUT_FILE"

	echo "Running: $1"
	eval "$1  2> \"$ERR_FILE\" 1> \"$OUT_FILE\""
	EXIT_CODE=$?

	echo "Expected exit code: $4"
	echo "Actual exit code: $EXIT_CODE"
	echo "Expected standard output:"
	(echo -n "$2" | cat -e ; echo)
	echo "Actual standard output:"
	(cat -e "$OUT_FILE" ; echo)	
	echo "Expected standard error:"
	(echo -n "$3" | cat -e ; echo)
	echo "Actual standard error:"
	(cat -e "$ERR_FILE" ; echo)
	if [ "$EXIT_CODE" -ne "$4" ]; then
		echo "[KO] Test failed : Expected exit code: $4"
		exit 1
	fi
	if [ "$2" != "$(cat "$OUT_FILE")" ]; then
		echo "[KO] Test failed : Expected standard output: \"$2\" "
		exit 1
	fi
	if [ "$3" != "$(cat "$ERR_FILE")" ]; then
		echo "[KO] Test failed : Expected standard error: \"$3\" "
		exit 1
	fi
	echo "[OK] Test passed"
	echo "------------------------"
	rm -f "$ERR_FILE" "$OUT_FILE"
}

#if $EXE not defined, then set it to ./ircserv
if [ -z "$EXE" ]; then
	EXE="./ircserv"
fi
