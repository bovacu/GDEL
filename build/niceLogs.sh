export GCC_COLORS="error=01;31:warning=01;33:note=01;36:locus=00;34"

nicePrint() {
	printf "\033[1;$1m$2  $3\033[0m\n"
}

buildSuccess() {
	printf '\n'
	nicePrint 32 "✔" "Succeeded!"
	printf '\n'
}

launching() {
	nicePrint 32 " " "Launching!!"
	printf '\n'
}

buildSuccessLaunch() {
	printf '\n'
	nicePrint 32 "✔" "Succeeded!"
	launching
}

buildError() {
	printf '\n'
	nicePrint 31 "✘" "Failed!"
	nicePrint 31 " " "Review the compile errors above."
	printf '\n'
	printf '\033[0m'
	exit 1
}

errorMessage() {
	printf '\n'
	nicePrint 31 "✘" "$1"
	printf '\n'
	printf '\033[0m'
	exit 1
}