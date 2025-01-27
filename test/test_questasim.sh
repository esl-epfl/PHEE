echo "========================"
echo "Test PHEE8 on Questasim"
echo "========================"
date
echo ""

make app PROJECT=posit8_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=on_chip

make sim-opt FUSESOC_FLAGS="--flag=use_posit8 --flag=use_quire"
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=0 execute_from_flash=0"
cat uart0.log

cd ../../..
make app PROJECT=posit8_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=flash_load
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/

make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=0"
cat uart0.log

cd ../../..
make app PROJECT=posit8_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=flash_exec
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/

make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=1"
cat uart0.log

cd ../../..
echo "========================"
echo "Test PHEE16 on Questasim"
echo "========================"
date
echo ""

make app PROJECT=posit16_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=on_chip

make sim-opt FUSESOC_FLAGS="--flag=use_posit16 --flag=use_quire"
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=0 execute_from_flash=0"
cat uart0.log

cd ../../..
make app PROJECT=posit16_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=flash_load
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/

make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=0"
cat uart0.log

cd ../../..
make app PROJECT=posit16_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=flash_exec
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/

make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=1"
cat uart0.log

cd ../../..
echo "========================"
echo "Test PHEE32 on Questasim"
echo "========================"
date
echo ""

make app PROJECT=posit32_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=on_chip

make sim-opt FUSESOC_FLAGS="--flag=use_posit32 --flag=use_quire"
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=0 execute_from_flash=0"
cat uart0.log

cd ../../..
make app PROJECT=posit32_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=flash_load
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/

make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=0"
cat uart0.log

cd ../../..
make app PROJECT=posit32_testsuite COMPILER=clang ARCH=rv32imcxposit1 TARGET=sim LINKER=flash_exec
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/

make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=1"
cat uart0.log

echo "=================="
echo "Finished all tests"
echo "=================="
date
echo ""