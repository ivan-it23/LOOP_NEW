param(
    [string]$OutputPath = (Join-Path (Split-Path -Parent $PSScriptRoot) 'METRO-CAPACITY-CALCULATIONS-COMBINED.png')
)

Add-Type -AssemblyName System.Drawing

$root = Split-Path -Parent $PSScriptRoot
$items = @(
    @{ File = 'METRO-AUTONOM-5TX.PNG';             Label = 'METRO-AUTONOM-5TX' },
    @{ File = 'METRO-CARTOGRAPH---LWD4TX.PNG';     Label = 'METRO-CARTOGRAPH---LWD4TX' },
    @{ File = 'METRO-LWD-3TX-V1.PNG';              Label = 'METRO-LWD-3TX-V1' },
    @{ File = 'METRO-LWD-3TX-V2.PNG';              Label = 'METRO-LWD-3TX-V2' },
    @{ File = 'METRO-LWD-4TX-V1.PNG';              Label = 'METRO-LWD-4TX-V1' },
    @{ File = 'METRO-LWD-4TX-V2.PNG';              Label = 'METRO-LWD-4TX-V2' }
)

$images = @()
try {
    foreach ($item in $items) {
        $path = Join-Path $root $item.File
        $images += [System.Drawing.Image]::FromFile($path)
    }

    $outerMargin = 32
    $columnGap = 32
    $rowGap = 36
    $labelHeight = 62
    $columnWidth = ($images | Measure-Object -Property Width -Maximum).Maximum
    $rowHeights = @(
        [Math]::Max($images[0].Height, $images[1].Height),
        [Math]::Max($images[2].Height, $images[3].Height),
        [Math]::Max($images[4].Height, $images[5].Height)
    )

    $canvasWidth = ($outerMargin * 2) + ($columnWidth * 2) + $columnGap
    $canvasHeight = ($outerMargin * 2) + ($labelHeight * 3) + ($rowHeights | Measure-Object -Sum).Sum + ($rowGap * 2)
    $canvas = New-Object System.Drawing.Bitmap($canvasWidth, $canvasHeight, [System.Drawing.Imaging.PixelFormat]::Format24bppRgb)

    try {
        $graphics = [System.Drawing.Graphics]::FromImage($canvas)
        try {
            $graphics.Clear([System.Drawing.Color]::FromArgb(244, 246, 248))
            $graphics.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
            $font = New-Object System.Drawing.Font('Arial', 28, [System.Drawing.FontStyle]::Bold, [System.Drawing.GraphicsUnit]::Pixel)
            $textBrush = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(25, 31, 38))
            $borderPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(156, 163, 175), 2)

            try {
                $y = $outerMargin
                for ($row = 0; $row -lt 3; $row++) {
                    for ($col = 0; $col -lt 2; $col++) {
                        $index = ($row * 2) + $col
                        $cellX = $outerMargin + ($col * ($columnWidth + $columnGap))
                        $image = $images[$index]
                        $imageX = $cellX + [Math]::Floor(($columnWidth - $image.Width) / 2)
                        $imageY = $y + $labelHeight

                        $graphics.DrawString($items[$index].Label, $font, $textBrush, $cellX, $y + 8)
                        $graphics.DrawImageUnscaled($image, $imageX, $imageY)
                        $graphics.DrawRectangle($borderPen, $imageX, $imageY, $image.Width - 1, $image.Height - 1)
                    }
                    $y += $labelHeight + $rowHeights[$row] + $rowGap
                }
            }
            finally {
                $font.Dispose()
                $textBrush.Dispose()
                $borderPen.Dispose()
            }
        }
        finally {
            $graphics.Dispose()
        }

        $canvas.Save($OutputPath, [System.Drawing.Imaging.ImageFormat]::Png)
        Write-Output $OutputPath
    }
    finally {
        $canvas.Dispose()
    }
}
finally {
    foreach ($image in $images) {
        $image.Dispose()
    }
}
