# NXP_FRDM-MCXN947_BSP

**IMPORTANT**
- This is a development repository for the **NXP FRDM-MCXN947 Board Support Pack (BSP)**. It demonstrates some CMSIS concepts, but it is not planned to release this pack directly from this repository. The concepts are proposals only.

- Further work on [project templates](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/templates) would require alignment on the Linker Script Management. The automatic linker script management needs a C Startup code. `main.c` should call the board setup functions which needs to be added.

- When using CMSIS-Toolbox 2.8.0 with the releated DFP, an error is generated as the `pack: NXP.MCXN947_DFP@19.0.0` contains an empty `<block/>` which causes a failure. As workaround remove the element `<block/>` in the DFP. See also [#2003: Debug Sequence with <block/> creates unexpected instance type](https://github.com/Open-CMSIS-Pack/devtools/issues/2003).

- This BSP uses the generator integration of the [**CMSIS-Toolbox to configure NXP devices with MCUXpresso Config Tools**](https://open-cmsis-pack.github.io/cmsis-toolbox/MCUXpressoConfig).

- This BSP contains in the folder [boards/frdmmcxn947/cmsis/templates/Device] additions that should be moved to the related DFP.

## CMSIS Additions

Directory                   | Description
:---------------------------|:--------------
[boards/frdmmcxn947/cmsis](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis) | Placeholder for the CMSIS additions.
[boards/frdmmcxn947/cmsis/doc](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/doc) | User Guides and Documentation
[boards/frdmmcxn947/cmsis/driver](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/drivers) | [CMSIS-Driver VIO](https://arm-software.github.io/CMSIS_6/latest/Driver/group__vio__interface__gr.html) implementation for FRDM-MCXN947
[boards/frdmmcxn947/cmsis/examples](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/examples/) | Example projects based on the [CMSIS Solution Project Format](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/YML-Input-Format.md) Examples
[boards/frdmmcxn947/cmsis/layers](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/layers) | Board layers for using the board with [CMSIS-Toolbox - Reference Applications](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/ReferenceApplications.md).
[boards/frdmmcxn947/cmsis/templates](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/templates) | Device specific project templates to start new csolution projects.

## Using the development repository

This development repository can be used in a local directory and [mapped as software pack](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/build-tools.md#install-a-repository) using for example `cpackget` with:

    cpackget add <path>/NXP.FRDM-MCXN947_BSP

## Generate software pack

The software pack is generated using bash shell scripts.

- Pack generator script (`gen_pack.sh`) based on [Open-CMSIS-Pack/gen-pack](https://github.com/Open-CMSIS-Pack/gen-pack) generates the software pack.
Run this script locally with:

      ./gen_pack.sh

## Issues

Please feel free to raise an [issue on GitHub](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/issues) to report misbehavior (i.e. bugs) or start discussions about enhancements. This
is your best way to interact directly with the maintenance team and the community.
We encourage you to append implementation suggestions as this helps to decrease the
workload of the maintenance team.
