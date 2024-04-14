/*
   Copyright (c) 2020, The LineageOS Project. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include "property_service.h"

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include "init_ks01lte.h"

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Trim;

// copied from build/tools/releasetools/ota_from_target_files.py
// but with "." at the end and empty entry
std::vector<std::string> ro_product_props_default_source_order = {
    "",
    "product.",
    "product_services.",
    "odm.",
    "vendor.",
    "system.",
    "system_ext.",
};

void set_rild_libpath(char const variant[])
{
    std::string libpath("/system/vendor/lib/libsec-ril.");
    libpath += variant;
    libpath += ".so";

    property_override("vendor.rild.libpath", libpath.c_str());
}

void cdma_properties(char const operator_alpha[],
        char const operator_numeric[],
        char const default_cdma_sub[],
        char const default_network[],
        char const rild_lib_variant[])
{
    // Dynamic CDMA Properties
    property_override("ro.cdma.home.operator.alpha", operator_alpha);
    property_override("ro.cdma.home.operator.numeric", operator_numeric);
    property_override("ro.telephony.default_cdma_sub", default_cdma_sub);
    property_override("ro.telephony.default_network", default_network);
    set_rild_libpath(rild_lib_variant);

    // Static CDMA Properties
    property_override("ril.subscription.types", "NV,RUIM");
    property_override("telephony.lteOnCdmaDevice", "1");
}

void gsm_properties(const char default_network[],
        char const rild_lib_variant[])
{
    set_rild_libpath(rild_lib_variant);

    // Dynamic GSM Properties
    property_override("ro.telephony.default_network", default_network);

    // Static GSM Properties
    property_override("telephony.lteOnGsmDevice", "1");
}

void property_override(char const prop[], char const value[], bool add)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

void set_ro_product_prop(char const prop[], char const value[])
{
    for (const auto &source : ro_product_props_default_source_order) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value, false);
    }
}

void vendor_load_properties()
{
    const std::string bootloader = GetProperty("ro.bootloader", "");

    const auto set_ro_product_prop = [](const std::string &source,
            const std::string &prop, const std::string &value) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    if (bootloader.find("I9506") == 0) {
        /* ks01lte */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/ks01ltexx/ks01lte:5.0.1/LRX22C/I9506XXUDRB1:user/release-keys");
            set_ro_product_prop(source, "device", "ks01ltexx");
            set_ro_product_prop(source, "model", "GT-I9506");
            set_ro_product_prop(source, "name", "ks01lte");
        }
        property_override("ro.build.description", "ks01ltexx-user 5.0.1 LRX22C I9506XXUDRB1 release-keys");
        property_override("ro.build.product", "ks01ltexx");
    } else if (bootloader.find("E330S") == 0) {
        /* ks01lteskt */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/ks01lteskt/ks01lte:5.0.1/LRX22C/E330SKSUDPI2:user/release-keys");
            set_ro_product_prop(source, "device", "ks01lteskt");
            set_ro_product_prop(source, "model", "SHV-E330S");
            set_ro_product_prop(source, "name", "ks01lteskt");
        }
        property_override("ro.build.description", "ks01lteskt-user 5.0.1 LRX22C E300SKSUDPI2 release-keys");
        property_override("ro.build.product", "ks01lteskt");
    } else if (bootloader.find("E330K") == 0) {
        /* ks01ltektt */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/ks01ltektt/ks01lte:5.0.1/LRX22C/E330KKKUDPH1:user/release-keys");
            set_ro_product_prop(source, "device", "ks01ltektt");
            set_ro_product_prop(source, "model", "SHV-E330K");
            set_ro_product_prop(source, "name", "ks01ltektt");
        }
        property_override("ro.build.description", "ks01ltektt-user 5.0.1 LRX22C E330KKKUDPH1 release-keys");
        property_override("ro.build.product", "ks01ltektt");
    } else if (bootloader.find("E330L") == 0) {
        /* ks01ltelgt */
        for (const auto &source : ro_product_props_default_source_order) {
            set_ro_product_prop(source, "build.fingerprint", "samsung/ks01ltelgt/ks01lte:5.0.1/LRX22C/E330LKLUDPH4:user/release-keys");
            set_ro_product_prop(source, "device", "ks01ltelgt");
            set_ro_product_prop(source, "model", "SHV-E330L");
            set_ro_product_prop(source, "name", "ks01ltelgt");
        }
        property_override("ro.build.description", "ks01ltelgt-user 5.0.1 LRX22C E330LKLUDPH4 release-keys");
        property_override("ro.build.product", "ks01ltelgt");
    }

    const std::string device = GetProperty("ro.product.device", "");
    LOG(INFO) << "Found bootloader " << bootloader << ". " << "Setting build properties for " << device << ".\n";
}
