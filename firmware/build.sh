cd start/gcc
echo "Building..."
make 2>&1 > /dev/null
clear
echo "Done."
cp -p AtmelStart.bin ../../firmware.bin
echo "Cleanup..."
# make clean and hide the output
make clean 2>&1 > /dev/null
cd ../../
find . -name "*.d" -delete
echo "Done."