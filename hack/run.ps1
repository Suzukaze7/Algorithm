#Set-ExecutionPolicy Unrestrict

$src = $args[0] + ".cpp"
g++ $src -std=c++17 -DLTNS -include debug.cpp
if ($args.Length -eq 1) {
    ./a.exe
} else {
    $in = [string]$args[1] + ".in"
    Get-Content $in | ./a.exe
}