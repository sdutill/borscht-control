param(
    [switch]$clean
)

# Clean build if requested
if ($clean -and (Test-Path "build")) {
    Remove-Item -Recurse -Force build
}

# Create build directory if it doesn't exist
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Force -Path "build"
}

# Navigate to build directory, run cmake, and build
Push-Location build
cmake ..
cmake --build .
Pop-Location

# Print success message
Write-Host "Build complete! Executables are in the exe directory."