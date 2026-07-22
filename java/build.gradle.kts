plugins {
    java
    application
}

group = "sunnyland"
version = "0.1"

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

application {
    mainClass.set("sunnyland.Main")
    // macOS: GLFW must run on the first thread
    if (org.gradle.internal.os.OperatingSystem.current().isMacOsX) {
        applicationDefaultJvmArgs = listOf("-XstartOnFirstThread")
    }
}

repositories {
    mavenCentral()
}

val lwjglVersion = "3.3.6"
val lwjglNatives = "natives-macos-arm64"

dependencies {
    implementation(platform("org.lwjgl:lwjgl-bom:$lwjglVersion"))

    implementation("org.lwjgl:lwjgl")
    implementation("org.lwjgl:lwjgl-glfw")
    implementation("org.lwjgl:lwjgl-opengl")
    implementation("org.lwjgl:lwjgl-stb")

    runtimeOnly("org.lwjgl:lwjgl::$lwjglNatives")
    runtimeOnly("org.lwjgl:lwjgl-glfw::$lwjglNatives")
    runtimeOnly("org.lwjgl:lwjgl-opengl::$lwjglNatives")
    runtimeOnly("org.lwjgl:lwjgl-stb::$lwjglNatives")
}

tasks.named<JavaExec>("run") {
    if (org.gradle.internal.os.OperatingSystem.current().isMacOsX) {
        jvmArgs("-XstartOnFirstThread")
    }
    workingDir = projectDir
}
