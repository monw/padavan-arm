##################################################################
# Board PID      # Board Name     # PRODUCT # Note
##################################################################
# RAX3000M-NAND  # RAX3000M-NAND  # MT7981  #
##################################################################

CFLAGS += -DBOARD_MT3000 -DBOARD_MT7615_DBDC
BOARD_NUM_USB_PORTS=1
CONFIG_BOARD_RAM_SIZE=512

USE_IPV6=1