cd ../../../hw/vendor/esl-epfl_x-heep/hw/vendor/lowrisc_opentitan/hw/dv/dpi/uartdpi/
cc -shared -Bsymbolic -fPIC -o uartdpi.so -lutil uartdpi.c
cd -
