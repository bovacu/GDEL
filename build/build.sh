BUILD_TYPE=$1
APP_NAME=$2
NO_RUN=$3

source ./build/niceLogs.sh

execBuildRun() {
	if [ -z $BUILD_TYPE ]; then
    	nicePrint 33 "➖" "Build type default: DEBUG"
		BUILD_TYPE=DEBUG
	else
		if [[ $BUILD_TYPE == 'RELEASE' || $BUILD_TYPE == 'DEBUG' ]]; then
			nicePrint 33 "➖" "Build type: $BUILD_TYPE"
		else
			errorMessage "First argument must be DEBUG or RELEASE"
		fi
	fi

	if [ -z $APP_NAME ]; then
    	nicePrint 33 "➖" "App name default: app"
		APP_NAME=GDEL
	else
		nicePrint 33 "➖" "App name: $APP_NAME"
	fi

	nicePrint 33 "⏳" "Build started..."

	FINAL_DIR='../releases/debug'
	
	if [ "$BUILD_TYPE" == "RELEASE" ]; then
		FINAL_DIR='../releases/release'
	fi

	RUN="false"
	if [ -z $NO_RUN ]; then
		RUN="true"
	fi

	compile $BUILD_TYPE $APP_NAME $FINAL_DIR $RUN
}

compile() {
	cmakeOut=$(cd build; cmake .. -DBUILD_TYPE_PARAM=$1 -DAPP_NAME_PARAM=$2 -Wno-dev -Wno-switch)
	if [[ $? != 0 ]]; then
    	# There was an error
    	buildError
	else
    	# Compilation successfull
		makeOut=$(cd build; make)
		if [[ $? != 0 ]]; then
			buildError
		else
			cd build
			mv $2-$1 $3
			PATH=`echo "$1" | tr '[:upper:]' '[:lower:]'`
			buildSuccess
			if [ "$4" == "true" ]; then
				nicePrint 35 "" "Running!!" 
				echo ""
				echo ""
				./../releases/$PATH/$2-$1
			fi
		fi
	fi
}

execBuildRun