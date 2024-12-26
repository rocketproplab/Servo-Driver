rm firmware.bin
cd start/gcc
echo "Building..."
make
echo "Done."
cp -p AtmelStart.bin ../../firmware.bin
echo "Cleanup..."
make clean
cd ../../
find . -name "*.d" -delete
echo "Done."