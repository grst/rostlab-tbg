#!/bin/sh

./build_native.sh
ant debug
adb shell am force-stop org.rostlab.tbg
adb uninstall org.rostlab.tbg
adb install bin/rostlab_tbg-g.apk
adb shell am start -n org.rostlab.tbg/.RostlabActivity
