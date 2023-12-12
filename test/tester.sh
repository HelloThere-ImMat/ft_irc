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

files=("successLog" "wrongPass" "noPass" "longNickname" "longUsername" "emptyPass" "wrongNick" "mode1" "mode2")

#################
### FUNCTIONS ###
#################

function ircTest() {
	local port=$((PORT + $1))
	local infile=$2
	local outfile=$3
	local outref=$4

	./ircserv $port ${PASSWORD} &> ${outfile} &
	ircserv_pid=$!
	sleep 1
	cat ${infile} | sed 's/$/\r/g' | nc ${ADDRESS} $port >/dev/null &
	sleep 4
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

function Test()
{
	local i="$1"
	local inputs="$2"
	local outputs="$3"
	local ref="$4"
	ircTest "$i" "$inputs" "$outputs" "$ref"
	val=$?;
	if [ "${val}" -eq 0 ]; then
		echo -e "${GREEN}${files[$i]} : OK${NC}"
	else
		echo -e "${RED}${files[$i]} KO${NC}"
		if [[ "$VALGRIND" != "" ]]; then
				cat $LOG_FILE
		fi
	fi
	exit $val
}

###############
### SCRIPT ####
###############

inputs=($(put_format "$IN_FOLDER" ".in" "${files[@]}"))
outputs=($(put_format "$OUT_FOLDER" ".out" "${files[@]}"))
outputs_ref=($(put_format "$OUT_REF_FOLDER" ".ref" "${files[@]}"))

port_offset=0
ret_val=0
for i in "${!inputs[@]}"; do
	Test "$i" "${inputs[i]}" "${outputs[i]}" "${outputs_ref[i]}" &
	port_offset=$port_offset+1
done
wait
if [ "${ret_val}" -eq 0 ]; then
	exit 0
else
	exit 1
fi