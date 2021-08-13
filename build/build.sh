BUILD_TYPE=$1
RUN_TESTS=$2
APP_NAME=$3
NO_RUN=$4

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

	FINAL_DIR='../releases/debug'
	
	if [ "$BUILD_TYPE" == "RELEASE" ]; then
		FINAL_DIR='../releases/release'
	fi

	RUN="false"
	if [ -z $NO_RUN ]; then
		RUN="true"
	fi

	compile $BUILD_TYPE $RUN_TESTS $APP_NAME $FINAL_DIR $RUN
}

compile() {
	cmakeOut=$(cd build; cmake .. -DBUILD_TYPE_PARAM=$1 -DRUN_TESTS=$2 -DAPP_NAME_PARAM=$3 -Wno-dev -Wno-switch -Wnon-pod-varargs)
	if [[ $? != 0 ]]; then
    	# There was an error
    	buildError
	else
    	# Compilation successfull
		python build/cores.py
		cores=$?

		nicePrint 33 "⏳" "Build started with $cores cores..."
		makeOut=$(cd build; make -j$cores)
		if [[ $? != 0 ]]; then
			buildError
		else
			cd build
			mv $3-$1 $4
			PATH=`echo "$1" | tr '[:upper:]' '[:lower:]'`
			buildSuccess
			if [ "$5" == "true" ]; then
				nicePrint 35 "" "Running!!" 
				echo ""
				echo ""
				./../releases/$PATH/$3-$1
			fi
		fi
	fi
}

execBuildRun