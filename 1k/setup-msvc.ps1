param(
    $ver = '14.39'
)
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vs_installs = ConvertFrom-Json "$(&$vswhere -version '17.0' -format 'json')"
$vs_installs
$vs_installer = '${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\setup.exe'
$vs_path = $vs_installs[0].installationPath
$msvc_comp_id = "Microsoft.VisualStudio.Component.VC.$ver.17.9.x86.x64" # refer to: https://learn.microsoft.com/en-us/visualstudio/install/workload-component-id-vs-build-tools?view=vs-2022
echo "Installing $msvc_comp_id ..."
&$vs_installer modify --quiet --installPath $vs_path --add $msvc_comp_id | Out-Host

if($?) {
    echo 'setup msvc success.'
} else {
    echo 'setup msvc fail'
}

exit $LASTEXITCODE
