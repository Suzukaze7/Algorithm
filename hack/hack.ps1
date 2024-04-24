g++ input.cpp -o input.exe
g++ hacked.cpp -o hacked.exe
g++ std.cpp -o std.exe

for ($cnt = 1;; $cnt++) {
    Write-Host "`r${cnt}th hack" -ForegroundColor Green -NoNewline

    .\input.exe > data.in
    Get-Content data.in | .\hacked.exe > data.result
    Get-Content data.in | .\std.exe > data.out

    $out = Get-Content data.out | ForEach-Object {$_.Trim()}
    $result = Get-Content data.result | ForEach-Object {$_.Trim()}

    if (Compare-Object $out $result -CaseSensitive) {
        Write-Host "`nhack success" -ForegroundColor Red
        break
    }
}