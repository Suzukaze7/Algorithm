g++ input.cpp -o input.exe
g++ hacked.cpp -o hacked.exe
g++ spj.cpp -o spj.exe

for ($cnt = 1;; $cnt++) {
    Write-Host "`r${cnt}th hack" -ForegroundColor Green -NoNewline

    .\input.exe > data.in
    Get-Content data.in | .\hacked.exe > data.result
    Get-Content data.in, data.result | ./spj.exe > data.out

    $out = Get-Content data.out

    if ($out -ne "ok") {
        Write-Host "`nhack success: $out" -ForegroundColor Red
        break
    }
}