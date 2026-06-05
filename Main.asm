
; ============================================================
; File    : Main.asm
;--------------------------------------------------------------------
; Students Names:  Afnan Abbas, Ahmed Saeed, Fiyaz Ahmad
; Roll Nos : i24-2018, i24-2005, i24-2051
;--------------------------------------------------------------------
; Course  : Computer Organization And Assembly Language (COAL)
; Semester: Spring 2026
; Project : Dizzy Walk - Semester Project
; Submission Date  : 08/05/2026 [11:00 pm]

; Project Review   : Simulates a dizzy professor wandering a 2D maze with
;                    obstacles, pits, coins, key, wallet, file I/O, keyboard
;                    control, random walk, and visual console simulation.
;============================================================


.386
.MODEL flat, stdcall
.Stack 4096

INCLUDE Irvine32.inc

;Yeh macro text ka rang (foreground/background) badalne ke
;liye use hota hai aur EAX register ki value ko "push/pop" ke zariye safe rakhte hue naya color set kar deta hai.

; ============================================================
; MACRO: SetColor attr
; ============================================================
SetColor MACRO attr
    push  eax
    mov   eax, attr
    call  SetTextColor
    pop   eax
ENDM


; Windows API Beep function ka prototype
Beep PROTO, dwFreq:DWORD, dwDuration:DWORD

include Variables.inc
include DizzyWalk_Procedures.inc




; Yeh game ka main control center hai jahan se poori "Dizzy Walk" adventure shuru aur khatam hoti hai.
; Yeh intro screens dikhane ke baad aik loop chalata hai jo player ki movement, maze update, aur game-over conditions ko handle karta hai.
; Is procedure ke baghair hamari game ke saare hissay alag alag hotay; yehi wo jagah hai jo sab cheezon ko aik saath jorti hai.

; ============================================================
; MAIN FUNCTION
; ============================================================
main PROC
call Randomize
    ; Quick test to see if output works
    call  Clrscr

    mov   dh, 10
    mov   dl, 20
    call  Gotoxy
    SetColor 0Fh
    mov   edx, OFFSET msgStartHint
    call  WriteString

    call  ReadChar
    call  Clrscr

    call  DrawRetroSplash
    call  ReadChar           ; continue after splash
    call  SceneTransition

    call  SetupGameOptions
    call  SceneTransition
    call  InitMaze
    call  DrawMaze
    mov   eax, 800
    call  Delay

_gameLoop:
    cmp   gameOver, TRUE_VAL
    je    _gameEnd

    cmp   runMode, 1
    je    _noMax
    mov   eax, stepCount
    cmp   eax, stepLimit
    jl    _noMax
    SetColor COLOR_PIT
    mov   edx, OFFSET msgMaxSteps
    call  WriteString
    mov   gameOver, TRUE_VAL
    mov   endReason, 3
    call  Beep_GameOver
    jmp   _gameEnd

_noMax:
    call  RecordStep

    cmp   moveMode, 1
    je    _doKB
    call  DoRandomMove
    mov   eax, 120
    call  Delay
    jmp   _moved
_doKB:
    call  DoKeyboardMove
_moved:
    inc   stepCount
    call  CheckCell
    call  DrawMaze
    cmp   gameOver, TRUE_VAL
    jne   _gameLoop

_gameEnd:
    call  SaveAdventureLog
    call  SceneTransition
    call  ShowEndScreen
    call  WaitMsg
    SetColor COLOR_NORMAL
    call  ExitProcess
    ret
main ENDP

END main