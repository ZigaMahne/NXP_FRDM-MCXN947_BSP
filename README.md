# NXP_FRDM-MCXN947_BSP

This is the development repository for the **CMSIS additions** to the **NXP FRDM-MCXN947 Board Support Pack (BSP)**.

This BSP uses the generator integration of the [**CMSIS-Toolbox to configure NXP devices with MCUXpresso Config Tools**](https://github.com/Open-CMSIS-Pack/NXP_FRDM-MCXN947_BSP/tree/main/boards/frdmmcxn947/cmsis/doc/ConfigToolsGuide.md).

## CMSIS additions top-level structure

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
