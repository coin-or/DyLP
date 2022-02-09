# Usage: genDefForDylp.ps1 <objDir> <component>
# <objDir> should be $(IntDir) in VS
# <component> is OsiDylp
# Eventually, this script may expand to handle the base libDylp, but for now
# I'm going with a hand-crafted .def file there. Dylp is plain old C, and it's
# easy to maintain the .def file by hand.

$VSPathComponents = ,'C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE'
$VSPathComponents += 'C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\BIN'
$VSPathComponents += 'C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools'
$VSPathComponents += 'C:\Windows\Microsoft.NET\Framework\v3.5'
$VSPathComponents += 'C:\Windows\Microsoft.NET\Framework\v2.0.50727'
$VSPathComponents += 'C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\VCPackages'
$VSPathComponents += 'C:\Program Files\Microsoft SDKs\Windows\v6.0A\bin'

echo "Checking path for VS directories ..."
foreach ($VSComponent in $VSPathComponents)
{ if ("$env:PATH" -notlike "*$VSComponent*")
  { echo "Prepending $VSComponent"
    $env:PATH = "$VSComponent;"+$env:PATH } }

$objPath = $Args[0]
$tgtBase = $Args[1]

"Looking in $objPath ..."

$objFiles = get-item -path $objPath\OsiDylp*.obj
if (!$objfiles)
{ Out-Default -InputObject "No file selected"
  return 1 }
$babyString = ".*OsiDylp.*"

$libTgt = "lib"+$tgtBase
$tgtDLL = $libTgt+".dll"
$tgtDef = $libTgt+".def"
echo "Target is $tgtDLL"

$defFile = new-item -path $objPath -name $tgtDef -type "file" -force
add-content -path $defFile -value "LIBRARY $libTgt`r`n`r`nEXPORTS"

$tmpfile = "$objPath\gendef.tmp.out"
# "Using $tmpfile"
$totalSyms = 0
echo "Processing files ... "
foreach ($file in $objFiles)
{ # get-member -inputobject $file
  $fileBase = $file.basename
  write-output $fileBase
  
  # The following line works just fine when this script is invoked directly
  # from powershell, and indirectly from a cmd window. But when it's invoked
  # as the pre-link event in VS, VS does something to output redirection that
  # causes all output to appear in the VS output window. Sending the dumpbin
  # output to a file fixes the problem until I can figure out how to block
  # VS's redirection of output.
  # $symbols = dumpbin /symbols $file
  $junk = dumpbin /OUT:$tmpfile /symbols $file
  $symbols = get-content $tmpfile

  # Eliminate Static symbols. Likewise labels.
  $symbols = $symbols -notmatch '^.*Static[^|]*\|.*$'
  $symbols = $symbols -notmatch '^.*Label[^|]*\|.*$'
  
  # Trim off the junk fore and aft. Some lines have no trailing information,
  # hence the second pattern
  $symbols = $symbols -replace '^.*\| ([^ ]+) .*$','$1'
  $symbols = $symbols -replace '^.*\| ([^ ]+)$','$1'
  
  # Lines we want will have @@ in the decorated name
  $filteredSymbols = $symbols -like "*@@*"
  $symLen = $filteredSymbols.length
  "Grabbed $symLen symbols"
  
  # Anything with "...@@$$..." seems to be invalid (either an artifact or a
  # system routine). But on occasion (template classes) it seems that the
  # required signature (@@$$F -> @@) is needed but isn't generated. So let's
  # try synthesizing it on the fly here.
  $filteredSymbols = $filteredSymbols -replace '@@\$\$F','@@'
  $filteredSymbols = $filteredSymbols -notlike '*@@$$*'

  # Lines with symbols that start with _ look to be compiler artifacts. Some
  # are acceptable to the linker, some not. It doesn't seem necessary to
  # export any of them. There's considerable, but not total, overlap between
  # this class of symbols and the previous class.
  $filteredSymbols = $filteredSymbols -notmatch '^_.*'

  # These are not acceptable to the linker, no specific reason given. 
  $filteredSymbols = $filteredSymbols -notmatch '^\?\?.@\$\$.*'
  
  # Lines with symbols that start with ??_[EG] are deleting destructors
  # (whatever that is) and should not be exported.
  $filteredSymbols = $filteredSymbols -notmatch '^\?\?_[EG].*'
  $symLen = $filteredSymbols.length
  "Initial filtering leaves $symLen"

  # These next filter lines may be needed because of mixed C/C++ code (Dylp's
  # code base is C, then there's OsiDylp in C++).
  # There's another lot of symbols that appeared in OsiDylp starting with
  # $.*$, where the intervening text is unwind, pdata, or similar. See if we
  # can get rid of them.
  # $filteredSymbols = $filteredSymbols -notmatch '^\$[^$][^$]*\$'
  # And then there's another lot that seem to be run time data (rtcName,
  # rtcFrameData, rtcVarDesc). Where does this stuff come from?
  # $filteredSymbols = $filteredSymbols -notmatch 'Z\$rtc[FNV].*$'
  # And then a bunch of symbols that start with ?dtor$ or ?catch$.
  # $filteredSymbols = $filteredSymbols -notmatch '^\?dtor\$'
  # $filteredSymbols = $filteredSymbols -notmatch '^\?catch\$'
  # $symLen = $filteredSymbols.length
  # "C/C++ filtering leaves $symLen"
  
  # std:: is not our problem, but we have to be a little bit careful not
  # to throw out the baby with the bathwater. It's not possible (as best
  # I can see) to distinguish std:: in a parameter vs. std:: as the name
  # space for the method without knowing a lot more about the mangling
  # algorithm. Toss the bath, then retrieve the baby.
  $notStd = $filteredSymbols -notlike "*@std@@*"
  $bathwaterAndBaby = $filteredSymbols -like "*@std@@*"
  $baby = $bathwaterAndBaby -match "$babyString"
  $filteredSymbols = $notStd+$baby
  "Removing std:: leaves $symLen"
  $filteredSymbols = $filteredSymbols | sort-object -unique
  $symLen = $filteredSymbols.length
  "$symLen unique symbols"
  $totalSyms += $symLen
  add-content -path $defFile -value "`r`n; $fileBase"
  add-content -path $defFile -value $filteredSymbols
}

remove-item $tmpfile
"Collected $totalSyms symbols."

return
