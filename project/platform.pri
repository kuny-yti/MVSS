
win32 {
    winrt|wince{
         Platform = Platform_Phone
    }
    else{
         Platform = Platform_Win32
    }
}
else:unix{
    Platform = Platform_Unix
}
else:mac{
    macx{
         Platform = Platform_Mac
    }
    else:ios{
         Platform = Platform_IOS
    }
    else{
         Platform = Platform_Mac
    }
}
else:android{
    Platform = Platform_Android
}
else{ #posix
    Platform = Platform_Linux
}

debug{
BulidLevel = debug
}
else:release{
BulidLevel = release
}
