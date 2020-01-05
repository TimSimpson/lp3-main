param(
    [Parameter(Mandatory=$True, Position=0, ValueFromPipeline=$false)]
    [System.String]
    $conan_profile
)

New-Variable -Name root_dir -Option ReadOnly -Value "$($PSScriptRoot)\.."
New-Variable -Name output_dir -Option ReadOnly -Value "$root_dir\output"
New-Variable -Name profile_path -Option ReadOnly -Value "$PSScriptRoot\profiles\$conan_profile"
New-Variable -Name build_dir -Option ReadOnly -Value "$output_dir\$conan_profile"

if (-not (Test-Path $profile_path -PathType Leaf)) {
    write("Conan profile file not found at $profile_path")
    exit 1
}

if (-not (Test-Path -path $build_dir)) { md $build_dir }

function conan() {
    param([string[]] $conan_args)

    $proc = Start-Process conan -ArgumentList $conan_args `
        -WorkingDirectory $build_dir `
        -NoNewWindow -PassThru -Wait
    if ($proc.ExitCode -ne 0) {
        throw "Failed with ExitCode=$($proc.ExitCode)"
    }
}

conan("install", $root_dir, "-pr=$profile_path", "--build", "missing")
conan("build", $root_dir)
