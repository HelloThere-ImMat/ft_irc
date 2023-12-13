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
CACHE_FOLDER=${TEST_FOLDER}/cache
CACHE=${CACHE_FOLDER}/.cache
ADDRESS=127.0.0.1
PORT=6667
PASSWORD=lol
TIME=7

files=("successLog" "wrongPass" "noPass" "longNickname" "longUsername" "emptyPass" "wrongNick" "mode1" "mode2" "mode3" "topic1" "topic2" "topic3" "kick")

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
	sleep $TIME
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

function show_spinner() {
    local pid=$1
    local delay=0.1
    local spin='-\|/'

    echo -e $BLUE
    echo -n "Loading... "
    while [ -d /proc/"$pid" ]; do
        printf "%s" "${spin:i++%${#spin}:1}"
        sleep "$delay"
        printf "\b"
    done
	printf "\r\033[K"
    echo -e $NC
}
###############
### SCRIPT ####
###############

inputs=($(put_format "$IN_FOLDER" ".in" "${files[@]}"))
outputs=($(put_format "$OUT_FOLDER" ".out" "${files[@]}"))
outputs_ref=($(put_format "$OUT_REF_FOLDER" ".ref" "${files[@]}"))

# Tests
declare -a return_values=()  # Array to store return values of test cases
mkdir ${CACHE_FOLDER}

for i in "${!inputs[@]}"; do
	Test "$i" "${inputs[i]}" "${outputs[i]}" "${outputs_ref[i]}" &>> "${CACHE}$i" &
	return_values+=($!)  # Store the PID of each background process
done

# Show Spinner when loading
show_spinner $!

# Get the correct return value
for pid in "${return_values[@]}"; do
    wait "$pid"
    return_code=$?
    ret_val=$((ret_val + return_code))  # Accumulate return values
done

# Display Outputs
for i in "${!inputs[@]}"; do
	cat ${CACHE}$i
done

rm -fr ${CACHE_FOLDER}

# Exit with the right value
if [ "${ret_val}" -eq 0 ]; then
	exit 0
else
	exit 1
fi
