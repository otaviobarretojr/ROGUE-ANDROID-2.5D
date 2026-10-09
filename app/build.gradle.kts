plugins {
    id("com.android.application")
}

android {
    namespace = "com.otaviobarreto.rogue25d"
    compileSdk = 35
    ndkVersion = "27.0.12077973"

    defaultConfig {
        applicationId = "com.otaviobarreto.rogue25d"
        minSdk = 26
        targetSdk = 35
        versionCode = 4
        versionName = "0.2.1-safe-start"
        ndk {
            abiFilters += listOf("arm64-v8a")
        }
        externalNativeBuild {
            cmake {
                cppFlags += listOf("-std=c++20")
            }
        }
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    buildTypes {
        release { isMinifyEnabled = false }
    }
}
