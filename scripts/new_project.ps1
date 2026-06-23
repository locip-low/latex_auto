# new_project.ps1 — Scaffold a new project folder
# Usage: .\scripts\new_project.ps1 -Name "2025_CUMCM_A"

param(
    [Parameter(Mandatory=$true)]
    [string]$Name
)

$Root = Split-Path -Parent $PSScriptRoot
$ProjDir = Join-Path $Root $Name
$TemplateDir = "D:\BaiduNetdiskDownload\practice\bilibili\template"
$ScriptDir = $PSScriptRoot

if (Test-Path $ProjDir) {
    Write-Host "Project folder already exists: $ProjDir" -ForegroundColor Red
    exit 1
}

Write-Host "Creating project: $ProjDir" -ForegroundColor Cyan

New-Item -ItemType Directory -Path $ProjDir -Force | Out-Null
New-Item -ItemType Directory -Path "$ProjDir\figures" -Force | Out-Null
New-Item -ItemType Directory -Path "$ProjDir\code" -Force | Out-Null

Copy-Item "$TemplateDir\cumcmthesis.cls" $ProjDir
Copy-Item "$ScriptDir\main_template.tex" "$ProjDir\main.tex"
Set-Content -Path "$ProjDir\ref.bib" -Value "% References BibTeX file" -Encoding UTF8

Write-Host "Done! Project scaffolded at: $ProjDir" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "  1. Place paper.docx in the folder"
Write-Host "  2. Put images in figures/"
Write-Host "  3. Put code in code/"
Write-Host "  4. Fill ref.bib"
Write-Host "  5. Run:  .\scripts\compile.ps1 -ProjectDir '$ProjDir'"
