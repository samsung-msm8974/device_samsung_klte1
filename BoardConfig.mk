# Copyright (C) 2014-2016 The CyanogenMod Project
# Copyright (C) 2017-2021 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

DEVICE_PATH := device/samsung/ks01lte
COMMON_PATH := device/samsung/msm8974-common

# inherit from common msm8974-common
include device/samsung/msm8974-common/BoardConfigCommon.mk

# Bluetooth
BOARD_CUSTOM_BT_CONFIG := $(COMMON_PATH)/bluetooth/vnd_ks01lte.txt

# Build Fingerprint
BUILD_FINGERPRINT := samsung/ks01ltexx/ks01lte:5.0.1/LRX22C/I9506XXUDRB1:user/release-keys

# Enable SVELTE memory configuration
MALLOC_SVELTE := true

TARGET_SPECIFIC_HEADER_PATH := $(DEVICE_PATH)/include

# Kernel
TARGET_KERNEL_CONFIG := lineage_ks01lte_defconfig

# OTA
TARGET_OTA_ASSERT_DEVICE := ks01lte,ks01ltexx,GT-I9506,ks01lteskt,ks01ltektt,ks01ltelgt

# Partition
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 2390753280	  # Dont change it
BOARD_USERDATAIMAGE_PARTITION_SIZE := 12528352256

# Properties
TARGET_SYSTEM_PROP += $(DEVICE_PATH)/system.prop

# Init
TARGET_INIT_VENDOR_LIB := //$(DEVICE_PATH):libinit_ks01lte

TARGET_PROCESS_SDK_VERSION_OVERRIDE += \
    /system/vendor/bin/hw/android.hardware.sensors@1.0-service.samsung8974=22 \
    /system/vendor/bin/hw/rild=27

# NFC
BOARD_NFC_HAL_SUFFIX := msm8974

# Radio
BOARD_PROVIDES_LIBRIL := true

# NFC
include $(COMMON_PATH)/nfc/bcm2079x/board.mk

# Radio/RIL
include $(COMMON_PATH)/radio/single/board.mk

# inherit from the proprietary version
include vendor/samsung/ks01lte/BoardConfigVendor.mk
