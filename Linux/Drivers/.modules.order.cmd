cmd_/mnt/hgfs/CodeBase/Linux/Drivers/modules.order := {   echo /mnt/hgfs/CodeBase/Linux/Drivers/example.ko; :; } | awk '!x[$$0]++' - > /mnt/hgfs/CodeBase/Linux/Drivers/modules.order
