# PHEE: Posit enabled x-HEEp

System integrating [Coprosit](https://github.com/esl-epfl/Coprosit) into [X-HEEP](https://github.com/esl-epfl/x-heep) via the [eXtension interface](https://docs.openhwgroup.org/projects/openhw-group-core-v-xif).

![PHEE diagram](docs/PHEE_diagram_horizontal.drawio.svg)

## Setup

First, create the `core-v-mini-mcu` conda environment of X-HEEP. Check X-HEEP's `README` for more information.
Then, generate the files from X-HEEP to use the [cv32e40px](https://github.com/esl-epfl/cv32e40px) CPU.

~~~bash
conda activate core-v-mini-mcu

make mcu-gen CPU=cv32e40px
~~~

You will also need the Xposit compiler compatible with the PULP extensions.
You can find it in the [PULP-compatible branch of llvm-xposit](https://github.com/esl-epfl/llvm-xposit/tree/PULP-compatible).

## Compiling applications

To compile the posit testsuite applications in `sw/applications/`, run for example:

~~~bash
make app PROJECT=posit32_testsuite COMPILER=clang ARCH=rv32imcxposit1
~~~

Where `posit32_testsuite` can be replaced with any project in `sw/applications/`.

Remember to check the rest of the flags from the `make app` command as well, in particular `TARGET` and `LINKER`. You can find them in X-HEEP's documentation.

More information on how to building external applications leveraging X-HEEP's compilation flow can be found in the `eXtendingHEEP.md` documentation in X-HEEP.

If you need more memory for your application, you can increase the number of memory banks
with the `MEMORY_BANKS` argument of `mcu-gen`. For example:

~~~bash
make mcu-gen CPU=cv32e40px MEMORY_BANKS=4
~~~

## Posit configuration

To configure the posit size, specify the appropiate flag when running `make sim` or `make synth-pynq-z2`, i.e. `FUSESOC_FLAGS="--flag=use_posit32"`.

In any case, only values of `POSLEN` smaller or equal to `XLEN` are supported. E.g. you can only use posit32 or smaller in a 32-bit CPU.

You can also set flags to include log-approximate multiplication, division and square root units or to include quire operations.

The available flags are the following:
- Posit size: `use_posit16`, `use_posit32`, `use_posit64`
- Posit log-approximate units: `use_pos_log_mult`, `use_pos_log_div`, `use_pos_log_sqrt`
- Quire operations: `use_quire`

## Simulating on QuestaSim

To simulate PHEE using FuseSoC on QuestaSim run:

~~~bash
make sim FUSESOC_FLAGS="--flag=use_posit32 --flag=use_quire"
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/
make run PLUSARGS="c firmware=../../../sw/build/main.hex"
~~~

or for the HDL optimized version:

~~~bash
make sim-opt FUSESOC_FLAGS="--flag=use_posit32 --flag=use_quire"
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex"
~~~

In any of these cases change `run` with `run-gui` to open the QuestaSim GUI.

If you want to execute from flash memory, you will also need the `boot_sel=1 execute_from_flash=1`
flags when running. For example, to run with `flash_exec`:

~~~bash
make sim-opt FUSESOC_FLAGS="--flag=use_posit32 --flag=use_quire"
cd build/esl-epfl_ip_phee_0.0.1/sim-modelsim/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/build/main.hex boot_sel=1 execute_from_flash=1"
~~~

## Running on FPGA

We support FPGA synthesis to the pynq-z2 board.

To synthesize PHEE using FuseSoC on Vivado (tested on Vivado 2022.2) run:

~~~bash
make synth-pynq-z2 FUSESOC_FLAGS="--flag=use_posit32 --flag=use_quire"
~~~

Then to program the bitstream, open Vivado,

~~~text
open --> Hardware Manager --> Open Target --> Autoconnect --> Program Device
~~~

and choose the file `PHEE/build/esl-epfl_ip_phee_0.0.1/synth-pynq-z2/esl-epfl_ip_phee_0.0.1.bit`.

To run applications on it using the EPFL programmer first recompile the application to
target the `pynq-z2`. Then follow the instructions to [program the flash](https://x-heep.readthedocs.io/en/latest/How_to/ProgramFlash.html)
and then to [execute from flash](https://x-heep.readthedocs.io/en/latest/How_to/ExecuteFromFlash.html).

Finally, you can see the output of the application using picocom:

~~~bash
picocom -b 9600 -r -l --imap lfcrlf /dev/ttyUSB2
~~~

## Vendor

To update the vendorized repositories run:

~~~bash
make vendor
~~~

When revendorizing X-HEEP, remember to regenerate the files from X-HEEP to use the cv32e40px CPU as stated in the [Setup](#setup).

## Testing

You can run some basic simulation tests with:

~~~bash
make test
~~~

Then check the results in `test/test_questasim.log`. You should see a bunch of
`... test OK` lines for each of the configurations that are tested.
