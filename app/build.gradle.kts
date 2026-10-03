plugins {
    id("com.android.application")
}

android {
    namespace = "com.otaviobarreto.rogue25d"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.otaviobarreto.rogue25d"
        minSdk = 26
        targetSdk = 35
        versionCode = 1
        versionName = "0.1.0"
    }

    buildTypes {
        release {
            isMinifyEnabled = false
        }
    }
}
