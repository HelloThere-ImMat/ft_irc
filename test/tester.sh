#!/bin/bash

#################
### VARIABLES ###
#################

RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[34m"
NC="\033[0m"

TEST_FOLDER=./test/
IN_FOLDER=${TEST_FOLDER}/in/
OUT_FOLDER=${TEST_FOLDER}/out/
OUT_REF_FOLDER=${TEST_FOLDER}/ref/
ADDRESS=127.0.0.1
PORT=6667
PASSWORD=lol

files=("successLog" "wrongPass" "noPass" "longNickname" "longUsername" "emptyPass" "wrongNick")

#################
### FUNCTIONS ###
#################

function ircTest() {
	local infile=$1
	local outfile=$2
	local outref=$3

	./ircserv ${PORT} ${PASSWORD} &> ${outfile} &
	ircserv_pid=$!
	sleep 1
	cat ${infile} | sed 's/$/\r/g' | nc ${ADDRESS} ${PORT} >/dev/null &
	sleep 1
	kill -s SIGINT $ircserv_pid
	diff "${outfile}" "${outref}"
	return $?
}

function put_format()
{
	local folder="$1"
	local suffix="$2"
	local files=("${@:3}")

	for (( i=0; i<${#files[@]}; i++ )); do
		files[$i]="${folder}${files[$i]}${suffix}"
	done
	echo "${files[@]}"
}

###############
### SCRIPT ####
###############

inputs=($(put_format "$IN_FOLDER" ".in" "${files[@]}"))
outputs=($(put_format "$OUT_FOLDER" ".out" "${files[@]}"))
outputs_ref=($(put_format "$OUT_REF_FOLDER" ".ref" "${files[@]}"))

ret_val=0
for i in "${!inputs[@]}"; do
	if ircTest "${inputs[i]}" "${outputs[i]}" "${outputs_ref[i]}"; then
		echo -e "${GREEN}${files[i]} : OK${NC}"
	else
		ret_val+=$?
		echo -e "${RED}${files[i]} KO${NC}"
		if [[ "$VALGRIND" != "" ]]; then
				cat $LOG_FILE
		fi
	fi
done

if [ "${ret_val}" -eq 0 ]; then
	exit 0
else
	exit 1
fi