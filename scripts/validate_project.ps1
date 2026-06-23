# validate_project.ps1 -- Check project folder format for LaTeX typesetting
# Usage: powershell -ExecutionPolicy Bypass -File .\scripts\validate_project.ps1 -ProjectDir 'projects/test'

param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectDir,
    [switch]$Fix
)

$errors = @()
$warnings = @()
$info = @()

Write-Host "`n= = = Validate project: $ProjectDir = = =" -ForegroundColor Cyan
Write-Host ""

if (-not (Test-Path $ProjectDir)) {
    Write-Host "[X FATAL] Directory not found: $ProjectDir" -ForegroundColor Red
    exit 1
}

# ---- 1. Check .docx (root + one level deep) ----
$docxFiles = @(Get-ChildItem $ProjectDir -Filter *.docx -File -ErrorAction SilentlyContinue)
if ($docxFiles.Count -eq 0) {
    $subDirs = Get-ChildItem $ProjectDir -Directory -ErrorAction SilentlyContinue
    foreach ($sub in $subDirs) {
        $docxFiles += @(Get-ChildItem $sub.FullName -Filter *.docx -File -ErrorAction SilentlyContinue)
    }
}

if ($docxFiles.Count -eq 0) {
    $errors += "[X FATAL] Missing paper.docx -- Word document required"
}
elseif ($docxFiles.Count -gt 1) {
    $warnings += "[! WARN] Multiple .docx files found, using: $($docxFiles[0].Name)"
}
else {
    $info += "[OK] Paper document: $($docxFiles[0].Name)"
}

# ---- 2. Check figures/ ----
$figDir = Join-Path $ProjectDir 'figures'
if (-not (Test-Path $figDir)) {
    if ($Fix) {
        New-Item -ItemType Directory -Path $figDir -Force | Out-Null
        $info += "[OK] figures/ auto-created"
    } else {
        $warnings += "[! WARN] Missing figures/ directory -- run with -Fix to auto-create"
    }
}
else {
    $imgs = @(Get-ChildItem $figDir -File -ErrorAction SilentlyContinue | Where-Object { $_.Extension -match '\.(png|jpg|jpeg|pdf)$' })
    if ($imgs.Count -eq 0) {
        $warnings += "[! WARN] figures/ is empty, placeholders will be generated"
    } else {
        $info += "[OK] figures/ contains $($imgs.Count) image(s)"
    }
}

# ---- 3. Check code/ ----
$codeDir = Join-Path $ProjectDir 'code'
if (-not (Test-Path $codeDir)) {
    if ($Fix) {
        New-Item -ItemType Directory -Path $codeDir -Force | Out-Null
        $info += "[OK] code/ auto-created"
    } else {
        $warnings += "[! WARN] Missing code/ directory -- run with -Fix to auto-create"
    }
}
else {
    $codeFiles = @(Get-ChildItem $codeDir -File -ErrorAction SilentlyContinue)
    if ($codeFiles.Count -eq 0) {
        $warnings += "[! WARN] code/ is empty, lstinputlisting will fail during compile"
    } else {
        $info += "[OK] code/ contains $($codeFiles.Count) file(s): $($codeFiles.Name -join ', ')"
    }
}

# ---- 4. Check ref.bib (root + one level deep) ----
$bibPath = Join-Path $ProjectDir 'ref.bib'
if (-not (Test-Path $bibPath)) {
    $subDirs = Get-ChildItem $ProjectDir -Directory -ErrorAction SilentlyContinue
    foreach ($sub in $subDirs) {
        $candidate = Join-Path $sub.FullName 'ref.bib'
        if (Test-Path $candidate) { $bibPath = $candidate; break }
    }
}

if (-not (Test-Path $bibPath) -or (Split-Path $bibPath -Parent) -ne $ProjectDir) {
    if ($Fix) {
        Set-Content -Path (Join-Path $ProjectDir 'ref.bib') -Value '% References BibTeX file' -Encoding UTF8
        $info += "[OK] ref.bib auto-created (empty template)"
    } else {
        $warnings += "[! WARN] Missing ref.bib -- bibliography will not be generated"
    }
}
else {
    $bibContent = Get-Content $bibPath -Raw -ErrorAction SilentlyContinue
    if ([string]::IsNullOrWhiteSpace($bibContent) -or $bibContent.Trim() -eq '% References BibTeX file') {
        $warnings += "[! WARN] ref.bib is empty, please add BibTeX entries"
    } else {
        $info += "[OK] ref.bib contains content"
    }
}

# ---- 5. Check Word heading styles ----
if ($docxFiles.Count -gt 0) {
    try {
        $docxPath = $docxFiles[0].FullName
        Add-Type -AssemblyName System.IO.Compression.FileSystem
        $zip = [System.IO.Compression.ZipFile]::OpenRead($docxPath)
        $stylesEntry = $zip.GetEntry('word/styles.xml')
        if ($null -ne $stylesEntry) {
            $reader = New-Object System.IO.StreamReader($stylesEntry.Open())
            $stylesXml = $reader.ReadToEnd()
            $reader.Close()
            $hasHeadings = $stylesXml -match 'Heading|w:outlineLvl'
        } else {
            $hasHeadings = $false
        }
        $zip.Dispose()
        if (-not $hasHeadings) {
            $warnings += "[! WARN] Word doc has no heading styles -- pipeline will guess structure from numbered text"
        } else {
            $info += "[OK] Word doc contains heading styles"
        }
    } catch {
        $warnings += "[! WARN] Cannot check Word heading styles: $_"
    }
}

# ---- Output ----
Write-Host ""
if ($errors.Count -gt 0) {
    Write-Host "=== FATAL ERRORS (must fix) ===" -ForegroundColor Red
    $errors | ForEach-Object { Write-Host $_ -ForegroundColor Red }
}

if ($warnings.Count -gt 0) {
    Write-Host "`n=== WARNINGS (proceed with caution) ===" -ForegroundColor Yellow
    $warnings | ForEach-Object { Write-Host $_ -ForegroundColor Yellow }
}

if ($info.Count -gt 0) {
    Write-Host "`n=== PASSED ===" -ForegroundColor Green
    $info | ForEach-Object { Write-Host $_ -ForegroundColor Green }
}

Write-Host ""
if ($errors.Count -gt 0) {
    Write-Host "Result: FAIL -- fix errors above before typesetting" -ForegroundColor Red
    exit 1
}
elseif ($warnings.Count -gt 0) {
    Write-Host "Result: PASS with warnings -- can proceed (use -Fix for auto-fix)" -ForegroundColor Yellow
    exit 0
}
else {
    Write-Host "Result: PASS -- ready for typesetting" -ForegroundColor Green
    exit 0
}
