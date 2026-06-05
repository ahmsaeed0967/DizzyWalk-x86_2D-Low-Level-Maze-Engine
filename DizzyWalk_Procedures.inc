;--------------------------------------------------------------------
; Students Names:  Afnan Abbas, Ahmed Saeed, Fiyaz Ahmad
; Roll Nos : i24-2018, i24-2005, i24-2051
;--------------------------------------------------------------------

.code
;--------------------------------------
; Contains all the Procedures
;--------------------------------------


; Yeh procedure maze ki kisi bhi specific row aur column se character uthane ke liye banaya gaya hai.
; Formula (row * MAZE_COLS) + col use karke hum mazeGrid array mein se exact location dhoondte hain.
; Iske baghair humein kabhi nahi pata chalega ke Professor deewar ke samne khara hai ya kisi coin ke oopar.

; ============================================================
; PROCEDURE: GetMazeCell
;   IN:  EAX=row, EBX=col
;   OUT: AL = cell character
;   Preserves: EBX, ECX, EDX, ESI, EDI
;   FIX 4: use EDX as scratch; push/pop EBX explicitly.
; ============================================================
;-----------------------------------------------------------------------------------
GetMazeCell PROC
    push  esi
    push  edx
    push  ebx
    mov   edx, eax
    imul  edx, MAZE_COLS
    add   edx, ebx
    mov   esi, OFFSET mazeGrid
    add   esi, edx
    mov   al,  BYTE PTR [esi]
    pop   ebx
    pop   edx
    pop   esi
    ret
GetMazeCell ENDP
;-----------------------------------------------------------------------------------


; Yeh procedure maze ke kisi bhi khaas cell mein naya character (jaise coin ya trail) dalne ke liye use hota hai.
; Row aur column ko multiply karke index nikalte hain aur phir EDI register ke zariye mazeGrid mein naya value store karte hain.
; Iska faida ye hai ke jab professor move karta hai ya coin uthata hai, toh hum screen par cheezein update kar sakte hain.

; ============================================================
; PROCEDURE: SetMazeCell
;   IN: EAX=row, EBX=col, CL=char
; ============================================================
;-----------------------------------------------------------------------------------
SetMazeCell PROC
    push  edi
    push  eax
    push  ebx
    imul  eax, MAZE_COLS
    add   eax, ebx
    mov   edi, OFFSET mazeGrid
    add   edi, eax
    mov   BYTE PTR [edi], cl
    pop   ebx
    pop   eax
    pop   edi
    ret
SetMazeCell ENDP
;-----------------------------------------------------------------------------------


; Yeh procedure Professor ke samne wale 5 cells ko scan karta hai taake 3D status panel ko update kiya ja sakay.
; Facing direction ke mutabiq loop chalta hai aur pehla rukan (non-empty object) dhoond kar uska fasla record karta hai.
; Is se player ko andaza hota hai ke agay rasta saaf hai ya koi deewar ya cheez Professor ka rasta rokne wali hai.

; ============================================================
; PROCEDURE: ScanForwardObject
;   Looks ahead up to 5 cells in facing direction and records
;   nearest non-empty object for pseudo-3D status panel.
; ============================================================
;-----------------------------------------------------------------------------------
ScanForwardObject PROC
    push  eax
    push  ebx
    push  ecx
    push  edx
    push  esi
    push  edi

    mov   viewObjChar, ' '
    mov   viewObjDist, 0

    mov   esi, profRow
    mov   edi, profCol
    mov   ecx, 1

_sfoLoop:
    cmp   ecx, 6
    jge   _sfoDone

    mov   eax, facingDir
    cmp   eax, DIR_UP
    je    _sfoUp
    cmp   eax, DIR_DOWN
    je    _sfoDown
    cmp   eax, DIR_LEFT
    je    _sfoLeft
    inc   edi
    jmp   _sfoBounds
_sfoUp:
    dec   esi
    jmp   _sfoBounds
_sfoDown:
    inc   esi
    jmp   _sfoBounds
_sfoLeft:
    dec   edi

_sfoBounds:
    cmp   esi, 0
    jl    _sfoHitWall
    cmp   esi, MAZE_ROWS
    jge   _sfoHitWall
    cmp   edi, 0
    jl    _sfoHitWall
    cmp   edi, MAZE_COLS
    jge   _sfoHitWall

    mov   eax, esi
    mov   ebx, edi
    call  GetMazeCell
    cmp   al, CHAR_EMPTY
    je    _sfoNext
    cmp   al, CHAR_VISITED
    je    _sfoNext
    mov   viewObjChar, al
    mov   viewObjDist, ecx
    jmp   _sfoDone

_sfoHitWall:
    mov   viewObjChar, CHAR_WALL
    mov   viewObjDist, ecx
    jmp   _sfoDone

_sfoNext:
    inc   ecx
    jmp   _sfoLoop

_sfoDone:
    pop   edi
    pop   esi
    pop   edx
    pop   ecx
    pop   ebx
    pop   eax
    ret
ScanForwardObject ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure check karta hai ke Professor ke gird-o-nawah (surroundings) mein koi khatarnak pit mojood hai ya nahi.
; Charo taraf (Upar, Neechay, Dayein, Bayein) ke cells ko scan karke dekha jata hai ke wahan pit ka symbol toh nahi.
; Is se hum player ko "DANGER: PIT NEARBY!" ka warning message dikha sakte hain taake wo girne se bach jaye.

; ============================================================
; PROCEDURE: IsAdjacentToPit
;   OUT: EAX = 1 if any 4-neighbor is pit, else 0
; ============================================================
;-----------------------------------------------------------------------------------
IsAdjacentToPit PROC
    push  ebx
    push  ecx
    push  edx

    mov   eax, profRow
    mov   ebx, profCol
    cmp   eax, 0
    je    _iapDown
    dec   eax
    call  GetMazeCell
    cmp   al, CHAR_PIT
    je    _iapYes

_iapDown:
    mov   eax, profRow
    cmp   eax, MAZE_ROWS - 1
    jge   _iapLeft
    inc   eax
    mov   ebx, profCol
    call  GetMazeCell
    cmp   al, CHAR_PIT
    je    _iapYes

_iapLeft:
    mov   ebx, profCol
    cmp   ebx, 0
    je    _iapRight
    dec   ebx
    mov   eax, profRow
    call  GetMazeCell
    cmp   al, CHAR_PIT
    je    _iapYes

_iapRight:
    mov   ebx, profCol
    cmp   ebx, MAZE_COLS - 1
    jge   _iapNo
    inc   ebx
    mov   eax, profRow
    call  GetMazeCell
    cmp   al, CHAR_PIT
    je    _iapYes

_iapNo:
    mov   eax, 0
    jmp   _iapDone
_iapYes:
    mov   eax, 1
_iapDone:
    pop   edx
    pop   ecx
    pop   ebx
    ret
IsAdjacentToPit ENDP
;-----------------------------------------------------------------------------------



; ============================================================
; PROCEDURE: SceneTransition
;   Short wipe/loading effect between scenes.
; ============================================================
;-----------------------------------------------------------------------------------
SceneTransition PROC
    pushad
    call Clrscr
    mov ecx, 12
    mov dh, 8
_stLoop:
    mov dl, 16
    call Gotoxy
    mov eax, ecx
    test eax, 1
    jz _stEven
    SetColor 0Bh
    jmp _stColor
_stEven:
    SetColor 0Dh
_stColor:
    mov edx, OFFSET msgTransition
    call WriteString
    mov eax, 45
    call Delay
    inc dh
    loop _stLoop
    popad
    ret
SceneTransition ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure aik scene se doosre scene mein jane ke liye loading ya wipe effect create karta hai.
; Loop ke zariye colors switch hote hain aur loading message ki position badalti hai taake animation ki feel aaye.
; Is animation se player ko pata chalta hai ke game "hang" nahi hui balkay agla level ya scene load ho raha hai.

; ============================================================
; PROCEDURE: ShowEndScreen (For the MENU and ENDING SCENES)
; ============================================================
;-----------------------------------------------------------------------------------
ShowEndScreen PROC
    pushad
    call Clrscr

    ; === 1. DETERMINE WIN/LOSE COLOR LIVERY ===
    mov bl, 4Fh           ; Default Setup: Red BG + White FG (4Fh) for LOSE
    
    cmp endReason, 1
    jne _sesDrawBanner
    cmp profHasKey, TRUE_VAL
    jne _sesDrawBanner
    
    mov bl, 2Fh           ; Target Setup: Green BG + White FG (2Fh) for WIN

_sesDrawBanner:
    ; === 2. FULL WIDTH DYNAMIC BANNER ===
    mov dh, 2
    mov ecx, 3            ; Banner Height (3 lines)
_bannerRow:
    mov dl, 0
    call Gotoxy
    movzx eax, bl         ; Apply the chosen Win/Lose Color
    call SetTextColor
    push ecx
    mov ecx, 100          ; Console Width
_bannerCol:
    mov al, ' '           ; Paint solid color
    call WriteChar
    loop _bannerCol
    pop ecx
    inc dh
    loop _bannerRow

    ; === 3. WRITE WIN/LOSE TITLE ===
    mov dh, 3
    mov dl, 35
    call Gotoxy
    cmp bl, 2Fh           ; Was it Green (Win)?
    je _printWin
    
    mov eax, 4Eh          ; Red BG + Yellow Text
    call SetTextColor
    mov edx, OFFSET msgEndLoseTitle
    call WriteString
    jmp _sesReason
    
_printWin:
    mov eax, 2Eh          ; Green BG + Yellow Text
    call SetTextColor
    mov edx, OFFSET msgEndWinTitle
    call WriteString

_sesReason:
    ; === 4. DISPLAY REASON (Normal Background) ===
    mov eax, 0Fh          ; Reset to Black BG + White FG
    call SetTextColor
    
    mov dh, 7
    mov dl, 25
    call Gotoxy
    mov eax, 0Bh          ; Cyan
    call SetTextColor
    mov edx, OFFSET msgEndReasonLbl
    call WriteString
    
    mov eax, 0Eh          ; Yellow for reason text
    call SetTextColor
    mov eax, endReason
    cmp eax, 1
    je  _sesDest
    cmp eax, 2
    je  _sesPit
    cmp eax, 3
    je  _sesMax
    cmp eax, 4
    je  _sesQuit
    mov edx, OFFSET msgEndUnknown
    jmp _sesWrite
_sesDest:
    cmp profHasKey, FALSE_VAL
    jne _sesDestNormal
    mov eax, 0Ch          ; Light Red if key lost
    call SetTextColor
    mov edx, OFFSET msgLostKey
    jmp _sesWrite
_sesDestNormal:
    mov edx, OFFSET msgEndDest
    jmp _sesWrite
_sesPit:
    mov edx, OFFSET msgEndPit
    jmp _sesWrite
_sesMax:
    mov edx, OFFSET msgEndMax
    jmp _sesWrite
_sesQuit:
    mov edx, OFFSET msgEndQuit
_sesWrite:
    call WriteString

    ; === 5. DISPLAY FINAL STATS ===
    mov dh, 11
    mov dl, 25
    call Gotoxy
    mov eax, 0Ah          ; Light Green
    call SetTextColor
    mov edx, OFFSET msgArcadeScore
    call WriteString
    mov eax, 0Fh
    call SetTextColor
    mov eax, profWallet
    imul eax, 25
    call WriteDec

    mov dh, 12
    mov dl, 25
    call Gotoxy
    mov eax, 0Ah
    call SetTextColor
    mov edx, OFFSET msgArcadeCoins
    call WriteString
    mov eax, 0Fh
    call SetTextColor
    mov eax, profWallet
    call WriteDec

    mov dh, 13
    mov dl, 25
    call Gotoxy
    mov eax, 0Ah
    call SetTextColor
    mov edx, OFFSET msgArcadeSteps
    call WriteString
    mov eax, 0Fh
    call SetTextColor
    mov eax, stepCount
    call WriteDec

    ; Prompt Exit
    mov dh, 18
    mov dl, 30
    call Gotoxy
    mov eax, 0Dh          ; Light Magenta
    call SetTextColor
    mov edx, OFFSET msgEndPrompt
    call WriteString

    ; Final color reset
    mov eax, 0Fh
    call SetTextColor

    popad
    ret
ShowEndScreen ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure game shuru hone se pehle poora maze taiyar karta hai aur saare objects ko unki sahi jagah par set karta hai.
; Pehle grid ko khali karke boundary draw ki jati hai, phir loop ke zariye random aur static obstacles (walls, lakes, coins) place kiye jate hain.
; Is se har baar game aik fresh aur organized map ke sath start hoti hai taake player ko naya challenge mil sake.

; ============================================================
; PROCEDURE: InitMaze
; ============================================================
;-----------------------------------------------------------------------------------
InitMaze PROC
    push  eax
    push  ebx
    push  ecx
    push  edi

    ; Fill maze with empty cells
    mov   edi, OFFSET mazeGrid
    mov   ecx, MAZE_ROWS * MAZE_COLS
    mov   al,  CHAR_EMPTY
    rep   stosb

    ; ------------------------------------------------------------
    ; 1. ADDING MULTIPLE LAKES (~)
    ; ------------------------------------------------------------
    mov ecx, 15             ; Jitni lakes chahiye (Misaal ke taur par 15)
_loopLakes:
    mov eax, MAZE_ROWS * MAZE_COLS
    call RandomRange        ; Ek random index uthayega poori grid mein
    mov BYTE PTR [mazeGrid + eax], '~'
    loop _loopLakes

    ; ------------------------------------------------------------
    ; 2. ADDING MULTIPLE WALLS (#)
    ; ------------------------------------------------------------
    mov ecx, 25             ; Jitni walls chahiye hain
_loopWalls:
    mov eax, MAZE_ROWS * MAZE_COLS
    call RandomRange
    mov BYTE PTR [mazeGrid + eax], '#'
    loop _loopWalls


    ; ==========================================
    ; DRAWING THE BOUNDARY BOX 
    ; ==========================================
    mov esi, OFFSET mazeGrid
    
    ; 1. Top Wall aur Bottom Wall draw karna
    mov ecx, MAZE_COLS
_topBottomLoop:
    ; Top row
    mov eax, ecx
    dec eax
    mov BYTE PTR [esi + eax], CHAR_WALL
    
    ; Bottom row 
    mov ebx, MAZE_ROWS
    dec ebx
    imul ebx, MAZE_COLS
    add ebx, ecx
    dec ebx
    mov BYTE PTR [esi + ebx], CHAR_WALL
    
    loop _topBottomLoop

    ; 2. Left Wall aur Right Wall draw karna
    mov ecx, MAZE_ROWS
_leftRightLoop:
    mov eax, ecx
    dec eax
    imul eax, MAZE_COLS
    
    ; Left column
    mov BYTE PTR [esi + eax], CHAR_WALL
    
    ; Right column
    mov ebx, eax
    add ebx, MAZE_COLS
    dec ebx
    mov BYTE PTR [esi + ebx], CHAR_WALL
    
    loop _leftRightLoop
    ; ==========================================

    ; Place destination
    mov   eax, DEST_ROW
    mov   ebx, DEST_COL
    mov   cl,  CHAR_DEST
    call  SetMazeCell

    ; Horizontal wall: row=6, cols 5..14
    mov   ecx, 0
_w1:
    cmp   ecx, 10
    jge   _w1d
    mov   eax, 6
    mov   ebx, ecx
    add   ebx, 5
    mov   cl,  CHAR_WALL
    call  SetMazeCell
    inc   ecx
    jmp   _w1
_w1d:

    ; Vertical wall: col=18, rows 2..9
    mov   ecx, 0
_w2:
    cmp   ecx, 8
    jge   _w2d
    mov   eax, ecx
    add   eax, 2
    mov   ebx, 18
    mov   cl,  CHAR_WALL
    call  SetMazeCell
    inc   ecx
    jmp   _w2
_w2d:

    ; Horizontal wall: row=20, cols 30..39
    mov   ecx, 0
_w3:
    cmp   ecx, 10
    jge   _w3d
    mov   eax, 20
    mov   ebx, ecx
    add   ebx, 30
    mov   cl,  CHAR_WALL
    call  SetMazeCell
    inc   ecx
    jmp   _w3
_w3d:

    ; Building block: rows 12..14, cols 28..32
    mov   eax, 12
_bRow:
    cmp   eax, 15
    jge   _bDone
    mov   ebx, 28
_bCol:
    cmp   ebx, 33
    jge   _bColDone
    mov   cl,  CHAR_BUILDING
    call  SetMazeCell
    inc   ebx
    jmp   _bCol
_bColDone:
    inc   eax
    jmp   _bRow
_bDone:

    ; Lake: alternating rows 24/25, cols 8..20
    mov   ecx, 0
_lk:
    cmp   ecx, 13
    jge   _lkd
    mov   eax, 24
    test  ecx, 1
    jz    _lke
    inc   eax
_lke:
    mov   ebx, ecx
    add   ebx, 8
    mov   cl,  CHAR_LAKE
    call  SetMazeCell
    inc   ecx
    jmp   _lk
_lkd:

    ; Coins
    mov   ecx, 0
_pc:
    cmp   ecx, NUM_COINS
    jge   _pcd
    mov   eax, coinRow[ecx*4]
    mov   ebx, coinCol[ecx*4]
    mov   cl,  CHAR_COIN
    call  SetMazeCell
    inc   ecx
    jmp   _pc
_pcd:

    ; Pits
    mov   ecx, 0
_pp:
    cmp   ecx, NUM_PITS
    jge   _ppd
    mov   eax, pitRow[ecx*4]
    mov   ebx, pitCol[ecx*4]
    mov   cl,  CHAR_PIT
    call  SetMazeCell
    inc   ecx
    jmp   _pp
_ppd:

    ; Stumble squares
    mov   ecx, 0
_ps:
    cmp   ecx, NUM_STUMBLE
    jge   _psd
    mov   eax, stumbleRow[ecx*4]
    mov   ebx, stumbleCol[ecx*4]
    mov   cl,  CHAR_STUMBLE
    call  SetMazeCell
    inc   ecx
    jmp   _ps
_psd:

    pop   edi
    pop   ecx
    pop   ebx
    pop   eax
    ret
InitMaze ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure screen ke oopar aik professional Arcade-style HUD (Heads-Up Display) panel draw karta hai.
; Border symbols aur Gotoxy ki madad se ye Score, Wallet, aur Steps ko makhsoos columns mein set karta hai.
; Is panel ke baghair player ko andaza nahi hoga ke us ne kitne coins jama kiye hain ya wo kitne steps chal chuka hai.

; ============================================================
; PROCEDURE: DrawMaze
;   FIX 1: Gotoxy uses AL=col(byte), AH=row+2(byte) -- no
;           memory dereference.
;   FIX 2: ESI=row, EDI=col are stable throughout.  Cell char
;           stored in CL; no unbalanced push/pop of EAX.
; ============================================================

; ============================================================
; PROCEDURE: DrawHeaderPanel
; ============================================================
;-----------------------------------------------------------------------------------
DrawHeaderPanel PROC
    pushad
    
    ; Clear top area
    mov dh, 0
    mov dl, 0
    call Gotoxy
    
    ; Top Border
    SetColor 0Eh
    mov al, 201
    call WriteChar
    mov ecx, 70
_h_top:
    mov al, 205
    call WriteChar
    loop _h_top
    mov al, 187
    call WriteChar
    
    ; Middle Content
    mov dh, 1
    mov dl, 0
    call Gotoxy
    mov al, 186
    call WriteChar
    
    mov dl, 2
    call Gotoxy
    SetColor 0Bh
    mov edx, OFFSET msgArcadeHUD1
    call WriteString
    
    mov dl, 25
    call Gotoxy
    SetColor 0Fh
    mov edx, OFFSET msgArcadeScore
    call WriteString
    mov eax, profWallet
    imul eax, 25
    call WriteDec
    
    mov dl, 42
    call Gotoxy
    mov edx, OFFSET msgArcadeCoins
    call WriteString
    mov eax, profWallet
    call WriteDec
    
    mov dl, 58
    call Gotoxy
    mov edx, OFFSET msgArcadeSteps
    call WriteString
    mov eax, stepCount
    call WriteDec
    
    mov dl, 71
    call Gotoxy
    SetColor 0Eh
    mov al, 186
    call WriteChar
    
    ; Bottom Border of Header
    mov dh, 2
    mov dl, 0
    call Gotoxy
    mov al, 204
    call WriteChar
    mov ecx, 70
_h_bot:
    mov al, 205
    call WriteChar
    loop _h_bot
    mov al, 185
    call WriteChar
    
    popad
    ret
DrawHeaderPanel ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure maze ke right side par aik vertical status box draw karta hai jo game ki live information dikhata hai.
; Column 54 se start ho kar, ye registers aur variables (jaise moveMode, profHasKey) ko check karta hai aur dynamic text print karta hai.
; Is panel ki wajah se player ko har waqt apni position aur danger level ka pata rehta hai, jis se game interactive lagti hai.

; ============================================================
; PROCEDURE: DrawStatusPanel
;   Draws a vertical status box to the right of the maze
; ============================================================
;-----------------------------------------------------------------------------------
DrawStatusPanel PROC
    pushad
    
    ; Panel Start at Col 54
    mov dh, 4
    mov dl, 54
    call Gotoxy
    
    SetColor 0Bh
    mov al, 201
    call WriteChar
    mov ecx, 22
_s_top:
    mov al, 205
    call WriteChar
    loop _s_top
    mov al, 187
    call WriteChar
    
    ; Row 1: Mode
    mov dh, 5
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    SetColor 0Fh
    mov edx, OFFSET msgPanelMode
    call WriteString
    cmp moveMode, 1
    je _spKeyboard
    SetColor 0Ah
    mov edx, OFFSET msgPanelRandom
    jmp _spModeW
_spKeyboard:
    SetColor 0Eh
    mov edx, OFFSET msgPanelKeyboard
_spModeW:
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Bh
    mov al, 186
    call WriteChar
    
    ; Row 2: Run Type
    mov dh, 6
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    SetColor 0Fh
    mov edx, OFFSET msgPanelRun
    call WriteString
    cmp runMode, 1
    je _spEndless
    mov edx, OFFSET msgPanelLimited
    jmp _spRunW
_spEndless:
    mov edx, OFFSET msgPanelEndless
_spRunW:
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Bh
    mov al, 186
    call WriteChar

    ; Row 3: Key Status
    mov dh, 7
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    SetColor 0Fh
    mov edx, OFFSET msgKeyLabel
    call WriteString
    cmp profHasKey, TRUE_VAL
    jne _spNoKey
    SetColor 0Ah
    mov edx, OFFSET msgYes
    jmp _spKeyW
_spNoKey:
    SetColor 0Ch
    mov edx, OFFSET msgNo
_spKeyW:
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Bh
    mov al, 186
    call WriteChar

    ; Row 4: Danger Alert
    mov dh, 8
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    call IsAdjacentToPit
    cmp eax, 1
    jne _spSafe
    SetColor 0Ch
    mov edx, OFFSET msgDangerNear
    call WriteString
    jmp _spDangerW
_spSafe:
    SetColor 0Ah
    mov edx, OFFSET msgSafeStatus
    call WriteString
_spDangerW:
    mov dl, 77
    call Gotoxy
    SetColor 0Bh
    mov al, 186
    call WriteChar

    ; Row 5: Position
    mov dh, 9
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    SetColor 0Fh
    mov edx, OFFSET msgNameLabel ; Reuse "Name: " prefix for Pos
    mov edx, OFFSET msgLeftParen
    call WriteString
    mov eax, profRow
    call WriteDec
    mov edx, OFFSET msgCommaSpace
    call WriteString
    mov eax, profCol
    call WriteDec
    mov edx, OFFSET msgRightParen
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Bh
    mov al, 186
    call WriteChar

    ; Bottom Border
    mov dh, 10
    mov dl, 54
    call Gotoxy
    mov al, 200
    call WriteChar
    mov ecx, 22
_s_bot:
    mov al, 205
    call WriteChar
    loop _s_bot
    mov al, 188
    call WriteChar
    
    popad
    ret
DrawStatusPanel ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure screen par "Legend" ya "Controls" ka box banata hai taake player ko game ke buttons ka pata chal sakay.
; DH aur DL registers ki madad se box ki position set ki jati hai aur boundary symbols ke andar control messages print kiye jate hain.
; Is panel ke baghair player ko buttons aur symbols samajhne mein mushkil hogi, is liye ye aik choti "User Guide" ka kaam karta hai.

; ============================================================
; PROCEDURE: DrawLegendPanel
; ============================================================
;-----------------------------------------------------------------------------------
DrawLegendPanel PROC
    pushad
    
    ; Position below status
    mov dh, 12
    mov dl, 54
    call Gotoxy
    SetColor 0Dh
    mov al, 201
    call WriteChar
    mov ecx, 22
_l_top:
    mov al, 205
    call WriteChar
    loop _l_top
    mov al, 187
    call WriteChar
    
    mov dh, 13
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    SetColor 0Eh
    mov edx, OFFSET msgPanelControls
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Dh
    mov al, 186
    call WriteChar
    
    mov dh, 14
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    SetColor 07h
    mov edx, OFFSET msgPanelCtrl1
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Dh
    mov al, 186
    call WriteChar
    
    mov dh, 15
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    mov edx, OFFSET msgPanelCtrl2
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Dh
    mov al, 186
    call WriteChar
    
    mov dh, 16
    mov dl, 54
    call Gotoxy
    mov al, 186
    call WriteChar
    mov dl, 56
    call Gotoxy
    mov edx, OFFSET msgPanelCtrl3
    call WriteString
    mov dl, 77
    call Gotoxy
    SetColor 0Dh
    mov al, 186
    call WriteChar
    
    mov dh, 17
    mov dl, 54
    call Gotoxy
    mov al, 200
    call WriteChar
    mov ecx, 22
_l_bot:
    mov al, 205
    call WriteChar
    loop _l_bot
    mov al, 188
    call WriteChar
    
    popad
    ret
DrawLegendPanel ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure poori game screen ka "Artist" hai jo maze, UI panels, aur Professor ko console par paint karta hai.
; Nested loops (ESI aur EDI) ke zariye har cell ko check kiya jata hai aur object type ke mutabiq rang (SetColor) aur symbol draw hota hai.
; Is ke baghair hamari saari logic sirf registers mein hoti, yehi wo function hai jo code ko aik real "Game" ki shakal deta hai.

; ============================================================
; PROCEDURE: DrawMaze
; ============================================================
;-----------------------------------------------------------------------------------
DrawMaze PROC

    pushad

    ; 0. Clear Screen
    call Clrscr

    ; 1. Draw UI Panels
    call DrawHeaderPanel
    call DrawStatusPanel
    call DrawLegendPanel

    ; 2. Draw Scrolling Title (within the header gap if we want, or below)
    ; We'll put it at row 3 with scrolling offset
    mov   eax, stepCount
    and   eax, 07h ; reduced range to stay in panel
    mov   marqueeOffset, eax
    mov   dh,3
    mov   dl,15
    add   dl, BYTE PTR marqueeOffset
    call  Gotoxy
    mov   eax, stepCount
    test  eax, 1
    jz    _hdrEven
    SetColor 0Eh
    jmp   _hdrDraw
_hdrEven:
    SetColor 0Bh
_hdrDraw:
    mov edx,OFFSET msgTitleBar
    call WriteString

    ; 3. Draw Maze Top Border
    mov dh,4
    mov dl,0
    call Gotoxy
    SetColor 0Eh
    mov   al, 201
    call WriteChar
    mov ecx,MAZE_COLS
_topBorder:
    mov al,205
    call WriteChar
    loop _topBorder
    mov   al, 187
    call WriteChar

    ; 4. Draw Maze Body
    mov esi,0
_rowLoop:
    cmp esi,MAZE_ROWS
    jge _mazeDone

    ; Left border
    mov eax, esi
    mov dh, al
    add dh, 5 ; Offset for Header
    mov dl, 0
    call Gotoxy
    SetColor 0Eh
    mov   al,186
    call WriteChar

    mov edi,0
_colLoop:
    cmp edi,MAZE_COLS
    jge _endCols

    ; Cursor position
    mov eax, esi
    mov dh, al
    add dh, 5
    mov eax, edi
    mov dl, al
    inc dl
    call Gotoxy

    ; Professor?
    mov eax,profRow
    cmp eax,esi
    jne _notProf
    mov eax,profCol
    cmp eax,edi
    jne _notProf
    SetColor COLOR_PROF
    mov al,CHAR_PROF
    call WriteChar
    jmp _nextCell

_notProf:
    mov eax,esi
    mov ebx,edi
    call GetMazeCell

    ; Object drawing logic
    cmp al,CHAR_WALL
    jne _chkBuild
        SetColor 08h
    mov al,219
    call WriteChar
    jmp _nextCell
_chkBuild:
    cmp al,CHAR_BUILDING
    jne _chkLake
        SetColor 06h
    mov al,178
    call WriteChar
    jmp _nextCell
_chkLake:
    cmp al,CHAR_LAKE
    jne _chkPit
        SetColor 03h
    mov al,'~'
    call WriteChar
    jmp _nextCell
_chkPit:
    cmp al,CHAR_PIT
    jne _chkCoin
        SetColor 0Ch
    mov al,'X'
    call WriteChar
    jmp _nextCell
_chkCoin:
    cmp al,CHAR_COIN
    jne _chkDest
        SetColor 0Ah
    mov al,'$'
    call WriteChar
    jmp _nextCell
_chkDest:
    cmp al,CHAR_DEST
    jne _chkTrail
        SetColor 0Bh
    mov al,'D'
    call WriteChar
    jmp _nextCell
_chkTrail:
    cmp al,CHAR_VISITED
    jne _chkStumble
        SetColor 09h
    mov al,'.'
    call WriteChar
    jmp _nextCell
_chkStumble:
    cmp al,CHAR_STUMBLE
    jne _emptyCell
        SetColor 0Dh
    mov al,'!'
    call WriteChar
    jmp _nextCell
_emptyCell:
        SetColor 07h
    mov al,' '
    call WriteChar

_nextCell:
    inc edi
    jmp _colLoop

_endCols:
    ; Right border
    mov eax, esi
    mov dh, al
    add dh, 5
    mov dl, MAZE_COLS + 1
    call Gotoxy
    SetColor 0Eh
    mov   al,186
    call WriteChar

    inc esi
    jmp _rowLoop

_mazeDone:
    ; Bottom border
    mov eax, MAZE_ROWS
    add eax, 5
    mov dh, al
    mov dl,0
    call Gotoxy
    SetColor 0Eh
    mov   al,200
    call WriteChar
    mov ecx,MAZE_COLS
_bottomBorder:
    mov al,205
    call WriteChar
    loop _bottomBorder
    mov   al,188
    call WriteChar

    ; Legend at very bottom
    mov eax, MAZE_ROWS
    add eax, 7
    mov dh, al
    mov dl, 0
    call Gotoxy
    SetColor 07h
    mov edx, OFFSET msgLegend
    call WriteString

    popad
    ret

DrawMaze ENDP
;-----------------------------------------------------------------------------------


; Yeh procedure professor ki agli random direction decide karne ke liye 0 se 3 tak aik number generate karta hai.
; Is mein LCG algorithm use ho raha hai jo randSeed ko math calculations aur bitwise AND ke zariye makhsoos range mein rakhta hai.
; Is ke baghair random movement mumkin nahi thi, aur professor ka rasta har baar aik jaisa hi hota.

; ============================================================
; PROCEDURE: GetRandomDir
;   OUT: EAX = 0..3  (LCG)
; ============================================================
;-----------------------------------------------------------------------------------
GetRandomDir PROC
    push  edx
    mov   eax, randSeed
    imul  eax, 1664525
    add   eax, 1013904223
    mov   randSeed, eax
    and   eax, 3
    pop   edx
    ret
GetRandomDir ENDP
;-----------------------------------------------------------------------------------


; Yeh procedure professor ko maze mein ek step upar le jane ki zimmedari nibhata hai.
; Index calculation se agle cell ko check kiya jata hai aur agar rasta saaf ho toh profRow ko kam kar diya jata hai.
; Deewar ya rukawat ki surat mein ye sound bajata hai taake player ko pata chale ke rasta band hai.

; ============================================================
; PROCEDURE: MoveUp (Fixed Beep Logic)
; ============================================================
;-----------------------------------------------------------------------------------
MoveUp PROC
    push eax
    push ebx

    mov  eax, profRow
    cmp  eax, 0
    je   _playBeepUp

    dec  eax
    mov  ebx, MAZE_COLS
    mul  ebx
    add  eax, profCol

    movzx ebx, BYTE PTR [mazeGrid + eax]
    cmp  bl, '#'
    je   _playBeepUp
    cmp  bl, 'B'
    je   _playBeepUp
    cmp  bl, '~'
    je   _playBeepUp

    ; --- Success: Move and SKIP Beep ---
    dec  profRow
    jmp  _doneUp

_playBeepUp:
    INVOKE Beep, 400, 50

_doneUp:
    pop  ebx
    pop  eax
    ret
MoveUp ENDP
;-----------------------------------------------------------------------------------



; Is procedure ka kaam professor ko niche wale cell mein move karwana aur boundary check karna hai.
; Rasta saaf hone par profRow barh jati hai, warna code seedha Beep logic par jump kar jata hai.
; Yeh ensure karta hai ke professor maze se bahar na nikal jaye aur collisions ka sahi response mile.

; ============================================================
; PROCEDURE: MoveDown (Fixed Beep Logic)
; ============================================================
;-----------------------------------------------------------------------------------
MoveDown PROC
    push eax
    push ebx

    mov  eax, profRow
    inc  eax
    cmp  eax, MAZE_ROWS
    jge  _playBeepDown

    mov  ebx, MAZE_COLS
    mul  ebx
    add  eax, profCol

    movzx ebx, BYTE PTR [mazeGrid + eax]
    cmp  bl, '#'
    je   _playBeepDown
    cmp  bl, 'B'
    je   _playBeepDown
    cmp  bl, '~'
    je   _playBeepDown

    ; --- Success: Move and SKIP Beep ---
    inc  profRow
    jmp  _doneDown

_playBeepDown:
    INVOKE Beep, 400, 50

_doneDown:
    pop  ebx
    pop  eax
    ret
MoveDown ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure professor ki left side par movement ko handle karta hai aur column zero ka khayal rakhta hai.
; Current index mein se ek minus karke agla cell dekha jata hai taake deewar se takrane ka dar na rahe.
; Agar move successful ho jaye toh Beep ko skip kiya jata hai warna "thud" sound bajti hai.

; ============================================================
; PROCEDURE: MoveLeft (Fixed Beep Logic)
; ============================================================
;-----------------------------------------------------------------------------------
MoveLeft PROC
    push eax
    push ebx

    mov  eax, profCol
    cmp  eax, 0
    je   _playBeepLeft

    mov  eax, profRow
    mov  ebx, MAZE_COLS
    mul  ebx
    add  eax, profCol
    dec  eax

    movzx ebx, BYTE PTR [mazeGrid + eax]
    cmp  bl, '#'
    je   _playBeepLeft
    cmp  bl, 'B'
    je   _playBeepLeft
    cmp  bl, '~'
    je   _playBeepLeft

    ; --- Success: Move and SKIP Beep ---
    dec  profCol
    jmp  _doneLeft

_playBeepLeft:
    INVOKE Beep, 400, 50

_doneLeft:
    pop  ebx
    pop  eax
    ret
MoveLeft ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure professor ko maze ke right side par ek step agay barhane ke liye use hota hai.
; Maximum columns ki limit check ki jati hai aur collision na hone par profCol ko increment kiya jata hai.
; Yeh player ko movement ki freedom deta hai aur har rukawat par foran feedback provide karta hai.

; ============================================================
; PROCEDURE: MoveRight 
; ============================================================
;-----------------------------------------------------------------------------------
MoveRight PROC
    push eax
    push ebx

    mov  eax, profCol
    inc  eax
    cmp  eax, MAZE_COLS
    jge  _playBeepRight

    mov  eax, profRow
    mov  ebx, MAZE_COLS
    mul  ebx
    add  eax, profCol
    inc  eax

    movzx ebx, BYTE PTR [mazeGrid + eax]
    cmp  bl, '#'
    je   _playBeepRight
    cmp  bl, 'B'
    je   _playBeepRight
    cmp  bl, '~'
    je   _playBeepRight

    ; --- Success: Move and SKIP Beep ---
    inc  profCol
    jmp  _doneRight

_playBeepRight:
    INVOKE Beep, 400, 50

_doneRight:
    pop  ebx
    pop  eax
    ret
MoveRight ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure Professor ke har step ka record rakhta hai taake baad mein uska poora rasta (path) track kiya ja sakay.
; Current coordinates ko path arrays mein save karta hai aur peeche wale khali cell par "Visited" symbol (.) laga deta hai.
; Is trail logic ki wajah se player ko screen par Professor ka chora hua nishan nazar aata hai aur rasta yaad rehta hai.

; ============================================================
; PROCEDURE: RecordStep
;   FIX 3: guard stepCount > 0 before back-indexing path arrays.
; ============================================================
;-----------------------------------------------------------------------------------
RecordStep PROC
    push  eax
    push  ebx
    push  ecx

    mov   ecx, stepCount
    cmp   ecx, MAX_STEPS - 1
    jge   _rsDone

    ; Store current position
    mov   eax, profRow
    mov   pathRow[ecx*4], eax
    mov   eax, profCol
    mov   pathCol[ecx*4], eax

    ; Mark previous cell as trail (FIX 3: only when step > 0)
    cmp   ecx, 0
    je    _rsDone
    dec   ecx
    mov   eax, pathRow[ecx*4]
    mov   ebx, pathCol[ecx*4]
    push  ecx
    call  GetMazeCell
    pop   ecx
    cmp   al, CHAR_EMPTY
    jne   _rsDone
    mov   eax, pathRow[ecx*4]
    mov   ebx, pathCol[ecx*4]
    mov   cl,  CHAR_VISITED
    call  SetMazeCell

_rsDone:
    pop   ecx
    pop   ebx
    pop   eax
    ret
RecordStep ENDP
;-----------------------------------------------------------------------------------




; Yeh procedure maze ka "brain" hai jo har step ke baad check karta hai ke Professor kis cell ya object par khara hai.
; CMP instructions ke zariye ye faisla karta hai ke chabi girani hai (Lakes), game khatam karni hai (Pits), ya score barhana hai (Coins).
; Is logic ke baghair game mein koi challenge ya reward ka system nahi hota aur Professor sirf aik khali grid mein ghoomta rehta.

; ============================================================
; PROCEDURE: CheckCell (Updated for Key Loss)
; ============================================================
;-----------------------------------------------------------------------------------
CheckCell PROC
    pushad              ; Saare registers save kar lo

    ; 1. Current Position Index calculate karna
    mov eax, profRow
    imul eax, MAZE_COLS

    add eax, profCol
    movzx ebx, BYTE PTR [mazeGrid + eax]

    ; =========================================================
    ; A. SOLID & PENALTY OBJECTS (Lakes, Stumbles, Pits)
    ; =========================================================

    ; --- LAKE (~) ---
    cmp bl, '~'
    jne _notLake
    mov profHasKey, FALSE_VAL    ; Pani mein chabi gir gayi!
    mov edx, OFFSET msgDroppedKey
    call WriteString
    mov eax, 400                 ; Speed slow (400ms delay)
    call Delay
    jmp _updateUI

_notLake:
    ; --- STUMBLE (S or !) ---
    cmp bl, 'S'
    je _isStumble
    cmp bl, '!'
    jne _notStumble
_isStumble:
    mov profHasKey, FALSE_VAL    ; Thokar lagi, chabi gayi!
    mov edx, OFFSET msgDroppedKey
    call WriteString
    jmp _updateUI

_notStumble:
    ; --- PIT (X or O) ---
    cmp bl, 'X'
    je _isPit
    cmp bl, 'O'
    jne _notPit
_isPit:
    mov gameOver, TRUE_VAL
    INVOKE Beep, 200, 500   ; <--- Bohat bhari aur lambi sound (GameOver)
    mov endReason, 2             ; Reason: Fell in Pit
    jmp _exitCheck

_notPit:
    ; =========================================================
    ; B. COLLECTIBLES & GOALS (Coins, Home)
    ; =========================================================

    ; --- COIN (C or $) ---
    cmp bl, 'C'
    je _isCoin
    cmp bl, '$'
    jne _notCoin
_isCoin:
    inc profWallet               ; Paisa hi paisa!
    INVOKE Beep, 1200, 100  ; <--- High pitch 'ting' sound
    mov BYTE PTR [mazeGrid + eax], '*' ; 
    mov edx, OFFSET msgFoundCoin
    call WriteString
    jmp _updateUI

_notCoin:
    ; --- HOME / DESTINATION (H or D) ---
    cmp bl, 'H'
    je _isHome
    cmp bl, 'D'
    jne _updateUI
_isHome:
    mov gameOver, TRUE_VAL
    mov endReason, 1             ; Reached Destination
    ; Note: Win/Loss ka faisla ShowEndScreen mein profHasKey dekh kar hoga
    jmp _exitCheck

_updateUI:
    ; Yahan aapka Status Panel update karne ka code ayega
    ; Taake Key status (YES/NO) foran nazar aaye

_exitCheck:
    popad
    ret
CheckCell ENDP
;-----------------------------------------------------------------------------------




; ============================================================
; Sound stubs
; ============================================================

; Yeh procedure coin collect karne par ek halki aur teekhi "ting" sound bajata hai.
; EAX mein 1200 (pitch) aur EDX mein 100 (time) set karke Beep function ko data pass kiya jata hai.
; Is audio feedback se player ko coin milne ki khushi aur confirmation milti hai.

;-----------------------------------------------------------------------------------
Beep_Coin PROC
    push eax
    push edx
    mov  eax, 1200
    mov  edx, 100
    ; call Beep
    pop  edx
    pop  eax
    ret
Beep_Coin ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure game jeetne ya destination par pohanchne par victory sound bajata hai.
; Is mein frequency ko 800 aur duration ko 200 par set kiya gaya hai taake sound thori lambi aur wazay ho.
; Yeh jeetne wale lamhe ko mazeedaar banata hai aur player ko accomplishment ka ehsas dilata hai.

;-----------------------------------------------------------------------------------
Beep_Win PROC
    push eax
    push edx
    mov  eax, 800
    mov  edx, 200
    ; call Beep
    pop  edx
    pop  eax
    ret
Beep_Win ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure player ke haarne ya pit mein girne par ek bhari aur "fail" wali sound nikalta hai.
; 300 ki low frequency use ki gayi hai taake sound thori sad aur khatarnak mehsoos ho.
; Is se player ko foran pata chal jata hai ke game khatam ho chuki hai aur ab restart karna parega.

;-----------------------------------------------------------------------------------
Beep_GameOver PROC
    push eax
    push edx
    mov  eax, 300
    mov  edx, 300
    ; call Beep
    pop  edx
    pop  eax
    ret
Beep_GameOver ENDP
;-----------------------------------------------------------------------------------




; Yeh procedure professor ki "Dizzy" movement ko control karta hai jab wo khud se rasta dhoondne ki koshish karta hai.
; RandomRange use karke 0 se 3 tak aik number select hota hai jo decide karta hai ke professor agla step kis taraf lega.
; Is ke baghair professor khud se move nahi kar sakta tha, aur game ka "Dizzy Walk" wala maza khatam ho jata.

; ============================================================
; PROCEDURE: DoRandomMove
; ============================================================
;-----------------------------------------------------------------------------------
DoRandomMove PROC
    push eax
    
    mov eax, 4              ; 4 directions generate karega (0, 1, 2, 3)
    call RandomRange

    cmp eax, 0
    je  _drmUp
    cmp eax, 1
    je  _drmDown
    cmp eax, 2
    je  _drmLeft
    cmp eax, 3
    je  _drmRight
    jmp _drmEnd             ; Fallback

_drmUp:
    call MoveUp
    jmp _drmEnd
_drmDown:
    call MoveDown
    jmp _drmEnd
_drmLeft:
    call MoveLeft
    jmp _drmEnd
_drmRight:
    call MoveRight

_drmEnd:
    pop eax
    ret
DoRandomMove ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure player ko poora control deta hai taake wo WASD keys ke zariye professor ko apni marzi se chala sakay.
; ReadChar se input le kar check kiya jata hai ke konsi key dabai gayi hai, aur phir wahi movement function call hota hai.
; Is ke zariye player game mein actively participate karta hai aur khud rasta dhoond kar coins jama kar sakta hai.

; ============================================================
; PROCEDURE: DoKeyboardMove
; ============================================================
;-----------------------------------------------------------------------------------
DoKeyboardMove PROC
    push  edx
    call  ReadChar
    cmp   al, 'a'
    jl    _kbUp2
    cmp   al, 'z'
    jg    _kbUp2
    sub   al, 32
_kbUp2:
    cmp   al, 'W'
    je    _kbW
    cmp   al, 'S'
    je    _kbS
    cmp   al, 'A'
    je    _kbA
    cmp   al, 'D'
    je    _kbD
    cmp   al, 'R'
    je    _kbR
    cmp   al, 'Q'
    je    _kbQ
    mov   eax, 0
    jmp   _kbDone
_kbW:
    mov   facingDir, DIR_UP
    call  MoveUp
    jmp   _kbDone
_kbS:
    mov   facingDir, DIR_DOWN
    call  MoveDown
    jmp   _kbDone
_kbA:
    mov   facingDir, DIR_LEFT
    call  MoveLeft
    jmp   _kbDone
_kbD:
    mov   facingDir, DIR_RIGHT
    call  MoveRight
    jmp   _kbDone
_kbR:
    mov   moveMode, 0
    SetColor COLOR_NORMAL
    mov   edx, OFFSET msgSwitched
    call  WriteString
    mov   eax, 0
    jmp   _kbDone
_kbQ:
    SetColor COLOR_PIT
    mov   edx, OFFSET msgQuit
    call  WriteString
    mov   gameOver, TRUE_VAL
    mov   endReason, 4
    mov   eax, 0
_kbDone:
    pop   edx
    ret
DoKeyboardMove ENDP
;-----------------------------------------------------------------------------------




; Yeh procedure kisi bhi text string ko hamare main "file buffer" mein copy karne ke liye banaya gaya hai.
; ESI aur EDI registers ko pointer ke taur par use karke aik aik byte tab tak copy hota hai jab tak string khatam na ho jaye.
; Yeh File I/O ka ahem hissa hai kyunke is ke baghair hum log-file mein koi bhi information ya messages nahi likh sakte thay.

; ============================================================
; PROCEDURE: AppendStr   IN: EDX = null-terminated string
; ============================================================
;-----------------------------------------------------------------------------------
AppendStr PROC
    push  eax
    push  esi
    push  edi
    mov   esi, edx
    mov   edi, OFFSET fileBuffer
    add   edi, fileBufLen
_asL:
    mov   al, BYTE PTR [esi]
    cmp   al, 0
    je    _asD
    mov   BYTE PTR [edi], al
    inc   esi
    inc   edi
    inc   fileBufLen
    jmp   _asL
_asD:
    pop   edi
    pop   esi
    pop   eax
    ret
AppendStr ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure kisi bhi numeric value (jaise score ya steps) ko text mein convert karke file buffer mein save karta hai.
; Mathematical division (DIV 10) ke zariye digits alag kiye jate hain aur unhein ASCII characters bana kar buffer mein jora jata hai.
; Is ki madad se hum game khatam hone par player ka poora report card file mein professional tareeqe se likh sakte hain.

; ============================================================
; PROCEDURE: AppendDec   IN: EAX = value
; ============================================================
;-----------------------------------------------------------------------------------
AppendDec PROC
    push  eax
    push  ebx
    push  ecx
    push  edx
    push  edi
    mov   edi, OFFSET tempStr
    add   edi, 30
    mov   BYTE PTR [edi], 0
    dec   edi
    mov   ebx, 10
    cmp   eax, 0
    jne   _adL
    mov   BYTE PTR [edi], '0'
    dec   edi
    jmp   _adE
_adL:
    cmp   eax, 0
    je    _adE
    xor   edx, edx
    div   ebx
    add   dl,  '0'
    mov   BYTE PTR [edi], dl
    dec   edi
    jmp   _adL
_adE:
    inc   edi
    mov   edx, edi
    call  AppendStr
    pop   edi
    pop   edx
    pop   ecx
    pop   ebx
    pop   eax
    ret
AppendDec ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure game khatam hone par poora "Adventure Log" (record) aik file mein save karne ke liye banaya gaya hai.
; Pehle saara data (score, steps, path) buffer mein jama kiya jata hai aur phir Irvine32 functions ke zariye use permanent file mein likha jata hai.
; Is ki wajah se player apni game ki history aur rasta (coordinates) dekh sakta hai, bilkul aik real game report ki tarah.

; ============================================================
; PROCEDURE: SaveAdventureLog
;   FIX 7: mov edx, OFFSET szFilename restored before
;          CreateOutputFile.
; ============================================================
;-----------------------------------------------------------------------------------
SaveAdventureLog PROC
    push  eax
    push  ebx
    push  ecx
    push  edx

    mov   fileBufLen, 0
    mov   BYTE PTR fileBuffer, 0

    mov   edx, OFFSET msgAdventureLogTitle
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgNameLabel
    call  AppendStr
    mov   edx, OFFSET profName
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgRunTypeLabel
    call  AppendStr
    cmp   runMode, 1
    je    _slEndless
    mov   edx, OFFSET msgRunTypeLimited
    jmp   _slRunW
_slEndless:
    mov   edx, OFFSET msgRunTypeEndless
_slRunW:
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgStepLimitLabel
    call  AppendStr
    mov   eax, stepLimit
    call  AppendDec
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgWalletLabel
    call  AppendStr
    mov   eax, profWallet
    call  AppendDec
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgKeyLabel
    call  AppendStr
    cmp   profHasKey, TRUE_VAL
    je    _slKY
    mov   edx, OFFSET msgNo
    jmp   _slKW
_slKY:
    mov   edx, OFFSET msgYes
_slKW:
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgStepLabel
    call  AppendStr
    mov   eax, stepCount
    call  AppendDec
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgEndReasonLabel
    call  AppendStr
    mov   eax, endReason
    cmp   eax, 1
    je    _slERDest
    cmp   eax, 2
    je    _slERPit
    cmp   eax, 3
    je    _slERMax
    cmp   eax, 4
    je    _slERQuit
    mov   edx, OFFSET msgReasonUnknown
    jmp   _slERW
_slERDest:
    mov   edx, OFFSET msgReasonDest
    jmp   _slERW
_slERPit:
    mov   edx, OFFSET msgReasonPit
    jmp   _slERW
_slERMax:
    mov   edx, OFFSET msgReasonMax
    jmp   _slERW
_slERQuit:
    mov   edx, OFFSET msgReasonQuit
_slERW:
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgPathHeader
    call  AppendStr

    ; First 20 path entries
    mov   ecx, 0
    mov   eax, stepCount
    cmp   eax, 20
    jle   _slPM
    mov   eax, 20
_slPM:
    mov   ebx, eax
_slPL:
    cmp   ecx, ebx
    jge   _slPD
    mov   edx, OFFSET msgLeftParen
    call  AppendStr
    mov   eax, pathRow[ecx*4]
    call  AppendDec
    mov   edx, OFFSET msgCommaSpace
    call  AppendStr
    mov   eax, pathCol[ecx*4]
    call  AppendDec
    mov   edx, OFFSET msgRightParen
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    inc   ecx
    jmp   _slPL
_slPD:
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    mov   edx, OFFSET msgTreasureHeader
    call  AppendStr
    mov   ecx, 0
_slTL:
    cmp   ecx, treasureCount
    jge   _slTD
    mov   edx, OFFSET msgLeftParen
    call  AppendStr
    mov   eax, treasureRow[ecx*4]
    call  AppendDec
    mov   edx, OFFSET msgCommaSpace
    call  AppendStr
    mov   eax, treasureCol[ecx*4]
    call  AppendDec
    mov   edx, OFFSET msgRightParen
    call  AppendStr
    mov   edx, OFFSET msgCRLF
    call  AppendStr
    inc   ecx
    jmp   _slTL
_slTD:

    ; FIX 7: provide filename pointer
    mov   edx, OFFSET szFilename
    call  CreateOutputFile
    cmp   eax, 0FFFFFFFFh
    je    _slErr
    mov   fileHandle, eax
    mov   eax, fileHandle
    mov   edx, OFFSET fileBuffer
    mov   ecx, fileBufLen
    call  WriteToFile
    mov   eax, fileHandle
    call  CloseFile
    SetColor COLOR_COIN
    mov   edx, OFFSET msgFileSaved
    call  WriteString
    jmp   _slDone
_slErr:
    SetColor COLOR_PIT
    mov   edx, OFFSET msgFileError
    call  WriteString
_slDone:
    SetColor COLOR_NORMAL
    pop   edx
    pop   ecx
    pop   ebx
    pop   eax
    ret
SaveAdventureLog ENDP
;-----------------------------------------------------------------------------------




; Yeh procedure game shuru hone se pehle player se poochta hai ke wo professor ko khud chalana chahta hai ya usay dizzy (random) chor dena chahta hai.
; ReadChar ke zariye input liya jata hai aur agar player '1' dabaye toh moveMode keyboard par set ho jata hai, warna default dizzy mode chalta hai.
; Is aik choti si choice se poori game ka style badal jata hai—aap khud pilot banna chahte hain ya sirf tamashai ban kar dekhna chahte hain!

; ============================================================
; PROCEDURE: AskMoveMode
; ============================================================
;-----------------------------------------------------------------------------------
AskMoveMode PROC
    push  eax
    push  edx
    SetColor COLOR_NORMAL
    mov   edx, OFFSET msgEnterMode
    call  WriteString
    call  ReadChar
    call  WriteChar
    mov   edx, OFFSET msgCRLF
    call  WriteString
    cmp   al, '1'
    je    _amKB
    mov   moveMode, 0
    jmp   _amDone
_amKB:
    mov   moveMode, 1
_amDone:
    pop   edx
    pop   eax
    ret
AskMoveMode ENDP
;-----------------------------------------------------------------------------------



; Yeh procedure player se run mode poochta hai ke game endless hogi ya phir makhsoos steps ki limit wali.
; User se input le kar ye step limit ko sanitize karta hai taake steps 1 se kam ya maximum limit se zyada na hon.
; Is setup ke baghair game ko ye pata nahi chal sakega ke kab player ki bari khatam karni hai aur kab tak chalte rehna hai.

; ============================================================
; PROCEDURE: AskRunModeAndStepLimit
; ============================================================
;-----------------------------------------------------------------------------------
AskRunModeAndStepLimit PROC
    push  eax
    push  edx
    SetColor COLOR_NORMAL
    mov   edx, OFFSET msgEnterRunMode
    call  WriteString
    call  ReadChar
    call  WriteChar
    mov   edx, OFFSET msgCRLF
    call  WriteString
    cmp   al, '1'
    je    _armEndless

    mov   runMode, 0
    mov   edx, OFFSET msgEnterStepLimit
    call  WriteString
    call  ReadInt
    cmp   eax, 1
    jge   _armAtLeast1
    mov   eax, 1
_armAtLeast1:
    cmp   eax, MAX_STEPS
    jle   _armSetLimit
    mov   eax, MAX_STEPS
_armSetLimit:
    mov   stepLimit, eax
    jmp   _armDone

_armEndless:
    mov   runMode, 1
    mov   stepLimit, MAX_STEPS

_armDone:
    mov   edx, OFFSET msgCRLF
    call  WriteString
    pop   edx
    pop   eax
    ret
AskRunModeAndStepLimit ENDP
;-----------------------------------------------------------------------------------




; Yeh procedure game ka main setup menu aur aik khubsurat blue banner screen par draw karta hai.
; Input logic ke zariye ye player ko Dizzy ya Keyboard mode aur Limited ya Endless run choose karne ka mauqa deta hai.
; Is ke baghair hamari game ki shuruat boring hoti; ye screen game ko aik professional "VIP" look aur customizable feel deti hai.

; ============================================================
; PROCEDURE: SetupGameOptions (VIP BANNER :)
; ============================================================
;-----------------------------------------------------------------------------------
SetupGameOptions PROC
    pushad
    call Clrscr

    ; === 1. FULL WIDTH BLUE BANNER ===
    mov dh, 1
    mov ecx, 3            ; Banner Height (3 lines)
_sgoBannerRow:
    mov dl, 0
    call Gotoxy
    mov eax, 1Fh          ; 10h (Blue BG) + 0Fh (White FG) = 1Fh
    call SetTextColor
    push ecx
    mov ecx, 100          ; Console Width fill
_sgoBannerCol:
    mov al, ' '           ; Paint solid color spaces
    call WriteChar
    loop _sgoBannerCol
    pop ecx
    inc dh
    loop _sgoBannerRow

    ; === 2. WRITE TITLE INSIDE BANNER ===
    mov dh, 2
    mov dl, 30            ; Centered Position
    call Gotoxy
    mov eax, 1Eh          ; Blue BG + Yellow FG
    call SetTextColor
    mov edx, OFFSET msgTitleBar
    call WriteString

    ; === 3. RESET COLOR & PRINT MENU OPTIONS ===
    mov eax, 0Fh          ; Black BG + White FG (Normal)
    call SetTextColor
    
    mov dh, 6
    mov dl, 0
    call Gotoxy
    mov eax, 0Bh          ; Cyan Text
    call SetTextColor
    mov edx, OFFSET msgMainMenu
    call WriteString

    ; === 4. INPUT LOGIC ===
    mov eax, 0Fh
    call SetTextColor
    call ReadChar
    call WriteChar
    mov edx, OFFSET msgCRLF
    call WriteString

    ; Defaults
    mov moveMode, 1
    mov runMode, 0
    mov stepLimit, MAX_STEPS

    cmp al, '1'
    je  _sgOpt1
    cmp al, '2'
    je  _sgOpt2
    cmp al, '3'
    je  _sgOpt3
    cmp al, '4'
    je  _sgOpt4
    
    mov eax, 0Ch          ; Light Red error
    call SetTextColor
    mov edx, OFFSET msgBadOption
    call WriteString
    mov eax, 0Fh
    call SetTextColor
    jmp _sgAskSteps

_sgOpt1:
    mov moveMode, 0
    mov runMode, 0
    jmp _sgAskSteps
_sgOpt2:
    mov moveMode, 0
    mov runMode, 1
    jmp _sgDone
_sgOpt3:
    mov moveMode, 1
    mov runMode, 0
    jmp _sgAskSteps
_sgOpt4:
    mov moveMode, 1
    mov runMode, 1
    jmp _sgDone

_sgAskSteps:
    mov eax, 0Eh          ; Yellow Text
    call SetTextColor
    mov edx, OFFSET msgEnterStepLimit
    call WriteString
    mov eax, 0Fh
    call SetTextColor
    call ReadInt
    cmp eax, 1
    jge _sgAtLeast1
    mov eax, 1
_sgAtLeast1:
    cmp eax, MAX_STEPS
    jle _sgSetLimit
    mov eax, MAX_STEPS
_sgSetLimit:
    mov stepLimit, eax

_sgDone:
    mov edx, OFFSET msgCRLF
    call WriteString
    popad
    ret
SetupGameOptions ENDP
;-----------------------------------------------------------------------------------




; Yeh procedure game shuru hone se pehle aik zabardast retro-style intro screen draw karta hai.
; Clouds, palm trees aur hero ko screen par makhsoos coordinates (Gotoxy) aur rangon ke sath set kiya jata hai.
; Is splash screen se player ko pehli nazar mein hi aik professional aur purani arcade games wali feel milti hai.

; ============================================================
; PROCEDURE: DrawRetroSplash
;   Draws a retro game-like intro screen in console.
; ============================================================
;-----------------------------------------------------------------------------------
DrawRetroSplash PROC
    pushad

    call  Clrscr

    ; Top HUD
    mov   dh, 0
    mov   dl, 18
    call  Gotoxy
    SetColor 0Eh
    mov   edx, OFFSET msgSplashScore
    call  WriteString

    ; Title lines
    mov   dh, 1
    mov   dl, 20
    call  Gotoxy
    SetColor 0Bh
    mov   edx, OFFSET msgSplashTitle
    call  WriteString

    mov   dh, 2
    mov   dl, 18
    call  Gotoxy
    SetColor 0Fh
    mov   edx, OFFSET msgSplashSub
    call  WriteString

    ; Clouds
    mov   dh, 4
    mov   dl, 6
    call  Gotoxy
    SetColor 07h
    mov   edx, OFFSET msgCloud
    call  WriteString

    mov   dh, 5
    mov   dl, 52
    call  Gotoxy
    mov   edx, OFFSET msgCloud
    call  WriteString

    ; Palm tree #1
    mov   dh, 8
    mov   dl, 8
    call  Gotoxy
    SetColor 0Ah
    mov   edx, OFFSET msgPalmTop
    call  WriteString
    mov   dh, 9
    mov   dl, 8
    call  Gotoxy
    mov   edx, OFFSET msgPalmMid
    call  WriteString
    mov   dh, 10
    mov   dl, 8
    call  Gotoxy
    SetColor 06h
    mov   edx, OFFSET msgPalmTrunk
    call  WriteString
    mov   dh, 11
    mov   dl, 8
    call  Gotoxy
    mov   edx, OFFSET msgPalmTrunk
    call  WriteString

    ; Palm tree #2
    mov   dh, 9
    mov   dl, 58
    call  Gotoxy
    SetColor 0Ah
    mov   edx, OFFSET msgPalmTop
    call  WriteString
    mov   dh, 10
    mov   dl, 58
    call  Gotoxy
    mov   edx, OFFSET msgPalmMid
    call  WriteString
    mov   dh, 11
    mov   dl, 58
    call  Gotoxy
    SetColor 06h
    mov   edx, OFFSET msgPalmTrunk
    call  WriteString
    mov   dh, 12
    mov   dl, 58
    call  Gotoxy
    mov   edx, OFFSET msgPalmTrunk
    call  WriteString

    ; Ground / water bands
    mov   dh, 18
    mov   dl, 5
    call  Gotoxy
    SetColor 0Eh
    mov   edx, OFFSET msgGround1
    call  WriteString
    mov   dh, 19
    mov   dl, 5
    call  Gotoxy
    SetColor 03h
    mov   edx, OFFSET msgGround2
    call  WriteString

    ; Hero and collectible
    mov   dh, 15
    mov   dl, 36
    call  Gotoxy
    SetColor 0Fh
    mov   edx, OFFSET msgHero
    call  WriteString

    mov   dh, 15
    mov   dl, 46
    call  Gotoxy
    SetColor 0Ah
    mov   edx, OFFSET msgCoinIcon
    call  WriteString

    ; Prompt
    mov   dh, 23
    mov   dl, 24
    call  Gotoxy
    SetColor 0D
    mov   edx, OFFSET msgSplashStart
    call  WriteString

    popad
    ret
DrawRetroSplash ENDP
;-----------------------------------------------------------------------------------
