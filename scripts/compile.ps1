# compile.ps1 — LaTeX 一键编译脚本
# 用法: .\scripts\compile.ps1 -ProjectDir "项目文件夹路径"
# 示例: .\scripts\compile.ps1 -ProjectDir "D:\vibecoding\latex_auto\2025国赛A题"

param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectDir,
    [string]$MainTex = "main.tex",
    [switch]$Clean
)

$TexBin = "D:\APP\texlive\2026\bin\windows"
$XeLaTeX = Join-Path $TexBin "xelatex.exe"
$BibTeX  = Join-Path $TexBin "bibtex.exe"

Push-Location $ProjectDir

try {
    $base = [System.IO.Path]::GetFileNameWithoutExtension($MainTex)

    Write-Host "=== Pass 1: XeLaTeX ===" -ForegroundColor Cyan
    & $XeLaTeX -interaction=nonstopmode $MainTex
    if ($LASTEXITCODE -ne 0) { Write-Host "XeLaTeX Pass 1 had errors, check log." -ForegroundColor Yellow }

    if (Test-Path "$base.aux") {
        $auxContent = Get-Content "$base.aux" -Raw
        if ($auxContent -match "\\citation\{") {
            Write-Host "=== Pass 2: BibTeX ===" -ForegroundColor Cyan
            & $BibTeX $base
        } else {
            Write-Host "No citations found, skipping BibTeX." -ForegroundColor Gray
        }
    }

    Write-Host "=== Pass 3: XeLaTeX ===" -ForegroundColor Cyan
    & $XeLaTeX -interaction=nonstopmode $MainTex

    Write-Host "=== Pass 4: XeLaTeX (final) ===" -ForegroundColor Cyan
    & $XeLaTeX -interaction=nonstopmode $MainTex

    if (Test-Path "$base.pdf") {
        Write-Host "SUCCESS: $base.pdf generated!" -ForegroundColor Green
    } else {
        Write-Host "FAILED: PDF not generated. Check $base.log" -ForegroundColor Red
    }

} finally {
    if ($Clean) {
        Write-Host "Cleaning auxiliary files..." -ForegroundColor Gray
        Remove-Item "$base.aux", "$base.bbl", "$base.blg", "$base.log",
                    "$base.out", "$base.toc", "$base.synctex.gz",
                    "$base.synctex.gz(busy)" -ErrorAction SilentlyContinue
    }
    Pop-Location
}
