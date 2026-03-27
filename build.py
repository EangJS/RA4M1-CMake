import os
import shutil
import subprocess
import sys

# ===========================
# User Configuration
# ===========================
PROJECT_DIR = os.path.abspath(os.path.dirname(__file__))  # CMakeLists.txt location
BUILD_DIR = os.path.join(PROJECT_DIR, "build")
CMAKE_GENERATOR = "Ninja"
BUILD_TYPE = "Debug"  # Change to "Release" if needed

# Paths for flashing (optional)
JLINK_DEVICE = "RA4M1"
JLINK_INTERFACE = "SWD"
JLINK_SPEED = "4000"  # kHz

# ===========================
# Helper Functions
# ===========================
def run(cmd, cwd=None):
    """Run a shell command and exit on error."""
    print(f"\n> Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        print(f"Error: command failed with exit code {result.returncode}")
        sys.exit(result.returncode)

def ensure_build_dir():
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
        print(f"Created build directory: {BUILD_DIR}")

def clean_build():
    """Delete the build directory completely."""
    if os.path.exists(BUILD_DIR):
        print(f"Cleaning build directory: {BUILD_DIR}")
        shutil.rmtree(BUILD_DIR)
    else:
        print("Build directory does not exist. Nothing to clean.")

# ===========================
# Step 1: Configure CMake
# ===========================
def configure_cmake():
    run([
        "cmake",
        f"-G{CMAKE_GENERATOR}",
        f"-DCMAKE_BUILD_TYPE={BUILD_TYPE}",
        f"-DCMAKE_TOOLCHAIN_FILE={os.path.abspath(os.path.join(PROJECT_DIR, 'toolchain-arm-gcc.cmake'))}",
        PROJECT_DIR
    ], cwd=BUILD_DIR)

# ===========================
# Step 2: Build with Ninja
# ===========================
def build_project():
    run(["ninja"], cwd=BUILD_DIR)

def flash_with_jlink():
    elf_file = os.path.join(BUILD_DIR, "ra4m1_project.elf")
    jlink_cmds = f"""
device {JLINK_DEVICE}
if {JLINK_INTERFACE}
speed {JLINK_SPEED}
loadfile {elf_file}
reset
go
exit
"""
    with open(os.path.join(BUILD_DIR, "jlink_script.jlink"), "w") as f:
        f.write(jlink_cmds.strip())
    run(["JLinkExe", "-CommandFile", "jlink_script.jlink"], cwd=BUILD_DIR)

# ===========================
# Main Automation
# ===========================
if __name__ == "__main__":
    if len(sys.argv) > 1:
        if sys.argv[1].lower() == "clean":
            clean_build()
            sys.exit(0)
        elif sys.argv[1].lower() == "flash":
            flash_with_jlink()
            sys.exit(0)

    ensure_build_dir()
    configure_cmake()
    build_project()
    
    # Uncomment to flash automatically
    # flash_with_jlink()
    print("\nBuild complete! ELF, BIN, and HEX files are ready in the build directory.")