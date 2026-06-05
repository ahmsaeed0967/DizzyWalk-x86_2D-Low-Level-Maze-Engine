;--------------------------------------------------------------------
; Students Names:  Afnan Abbas, Ahmed Saeed, Fiyaz Ahmad
; Roll Nos : i24-2018, i24-2005, i24-2051
;--------------------------------------------------------------------

; ============================================================
;  SYMBOLIC CONSTANTS
; ============================================================
MAZE_ROWS    EQU  30
MAZE_COLS    EQU  50
CENTER_ROW   EQU  15
CENTER_COL   EQU  25
DEST_ROW     EQU  2
DEST_COL     EQU  47
MAX_STEPS    EQU  500
NUM_COINS    EQU  8
NUM_PITS     EQU  4
NUM_STUMBLE  EQU  3

CHAR_EMPTY    EQU  '.'
CHAR_WALL     EQU  '#'
CHAR_PIT      EQU  'O'
CHAR_COIN     EQU  '$'
CHAR_DEST     EQU  'D'
CHAR_PROF     EQU  'P'
CHAR_STUMBLE  EQU  'S'
CHAR_VISITED  EQU  '*'
CHAR_BUILDING EQU  'B'
CHAR_LAKE     EQU  '~'

COLOR_NORMAL   EQU  0Fh
COLOR_PROF     EQU  0Eh
COLOR_COIN     EQU  0Ah
COLOR_PIT      EQU  0Ch
COLOR_WALL     EQU  08h
COLOR_DEST     EQU  0Bh
COLOR_TRAIL    EQU  09h
COLOR_STUMBLE  EQU  0Dh
COLOR_LAKE     EQU  03h
COLOR_BUILDING EQU  06h

DIR_UP    EQU  0
DIR_DOWN  EQU  1
DIR_LEFT  EQU  2
DIR_RIGHT EQU  3

TRUE_VAL  EQU  1
FALSE_VAL EQU  0

; ============================================================
;  DATA SEGMENT
; ============================================================
.data

szFilename    BYTE  "adventure_log.txt",0

profName      BYTE  "Professor Dizzy",0
profRow       DWORD CENTER_ROW
profCol       DWORD CENTER_COL
profWallet    DWORD 0
profHasKey    DWORD TRUE_VAL
stepCount     DWORD 0
gameOver      DWORD FALSE_VAL
moveMode      DWORD 0           ; 0=random, 1=keyboard
runMode       DWORD 0           ; 0=limited steps, 1=endless
stepLimit     DWORD MAX_STEPS
endReason     DWORD 0           ; 0=none,1=destination,2=pit,3=max-step,4=user-quit
facingDir     DWORD DIR_UP      ; camera/view direction for pseudo-3D panel

pathRow       DWORD MAX_STEPS DUP(0)
pathCol       DWORD MAX_STEPS DUP(0)
treasureCount DWORD 0
treasureRow   DWORD NUM_COINS DUP(0)
treasureCol   DWORD NUM_COINS DUP(0)

mazeGrid      BYTE  MAZE_ROWS * MAZE_COLS DUP(CHAR_EMPTY)

coinRow       DWORD  5, 10, 20, 25,  3, 14, 27,  8
coinCol       DWORD 10, 35,  5, 42, 30,  8, 20, 45
coinActive    DWORD  1,  1,  1,  1,  1,  1,  1,  1

pitRow        DWORD  8, 22,  4, 18
pitCol        DWORD 12, 38, 20,  5

stumbleRow    DWORD 11, 19,  7
stumbleCol    DWORD 22, 14, 40

msgWelcome    BYTE  "==============================",13,10
              BYTE  " DIZZY WALK - COAL SP2026",13,10
              BYTE  "==============================",13,10
              BYTE  "Controls (Keyboard Mode):",13,10
              BYTE  "  W=Up  S=Down  A=Left  D=Right",13,10
              BYTE  "  R=Switch to Random  Q=Quit",13,10
              BYTE  "Press ENTER to start...",13,10,0

msgSwitched   BYTE  "Mode switched.",13,10,0
msgReachedDest BYTE 13,10,"*** Professor reached the DESTINATION! ***",13,10,0
msgHasKey     BYTE  "Professor HAS the key -> Entered home! GAME OVER.",13,10,0
msgLostKey    BYTE  "Professor LOST the key -> Sleeping outside. GAME OVER.",13,10,0
msgFellPit    BYTE  13,10,"*** Professor FELL INTO A PIT! GAME OVER. ***",13,10,0
msgMaxSteps   BYTE  13,10,"*** Maximum steps reached! Professor gave up. ***",13,10,0
msgQuit       BYTE  13,10,"*** User quit the simulation. ***",13,10,0
msgDroppedKey BYTE  "  [Stumble! Professor dropped the key!]",13,10,0
msgFoundCoin  BYTE  "  [Found a coin! Wallet += 1]",13,10,0
msgStepLabel  BYTE  "Step: ",0
msgWalletLabel BYTE "  Wallet: ",0
msgKeyLabel   BYTE  "  Key: ",0
msgYes        BYTE  "YES",0
msgNo         BYTE  "NO ",0
msgCRLF       BYTE  13,10,0
msgFileSaved  BYTE  "Adventure log saved to adventure_log.txt",13,10,0
msgFileError  BYTE  "File write error.",13,10,0
msgLegend     BYTE  "Legend: P=Prof  D=Dest  $=Coin  O=Pit  #=Wall  ~=Lake  B=Bldg  S=Stumble  *=Trail",13,10,0
msgTitleBar   BYTE  "DIZZY WALK - 2D MAZE SIMULATION",0
msgEnterMode  BYTE  "Enter mode (0=Random, 1=Keyboard): ",0
msgEnterRunMode BYTE "Run type (0=Specific steps, 1=Endless): ",0
msgEnterStepLimit BYTE "Enter number of steps (1..500): ",0
msgAdventureLogTitle BYTE "===== DIZZY WALK ADVENTURE LOG =====",13,10,0
msgNameLabel  BYTE "Professor Name: ",0
msgRunTypeLabel BYTE "Run Type: ",0
msgRunTypeLimited BYTE "Specific number of steps",0
msgRunTypeEndless BYTE "Endless until destination/criteria",0
msgStepLimitLabel BYTE "Step Limit: ",0
msgPathHeader BYTE "Path (row,col) entries:",13,10,0
msgTreasureHeader BYTE "Treasures (coins) picked at:",13,10,0
msgEndReasonLabel BYTE "End Reason: ",0
msgReasonDest BYTE "Reached destination",0
msgReasonPit BYTE "Fell into pit",0
msgReasonMax BYTE "Step limit reached",0
msgReasonQuit BYTE "User quit",0
msgReasonUnknown BYTE "Unknown/Not Set",0
msgCommaSpace BYTE ", ",0
msgLeftParen BYTE "(",0
msgRightParen BYTE ")",0
msgMainMenu BYTE 13, 10
            BYTE "                      +========================================+", 13, 10
            BYTE "                      |           SELECT YOUR DIZZY MISSION    |", 13, 10
            BYTE "                      +========================================+", 13, 10
            BYTE "                      |                                        |", 13, 10
            BYTE "                      |  [1] Auto-Pilot   (Limited Steps)      |", 13, 10
            BYTE "                      |  [2] Auto-Pilot   (Endless Mode)       |", 13, 10
            BYTE "                      |  [3] Manual Walk  (Limited Steps)      |", 13, 10
            BYTE "                      |  [4] Manual Walk  (Endless Mode)       |", 13, 10
            BYTE "                      |                                        |", 13, 10
            BYTE "                      +========================================+", 13, 10
            BYTE "                           Enter your choice [1-4]: ", 0
msgBadOption   BYTE  "Invalid option. Default: Keyboard + Limited Steps.",13,10,0
msgPanelMode     BYTE "Mode:",0
msgPanelRandom   BYTE "RANDOM     ",0      ; <--- 2 spaces add ki hain taake 8 chars poore hon
msgPanelKeyboard BYTE "KEYBOARD        ",0      ; <--- Ye pehle se 8 chars ka hai

msgPanelRun      BYTE "Run:",0
msgPanelLimited  BYTE "LIMITED          ",0       ; <--- Dono 7 chars ke hain, koi issue nahi
msgPanelEndless  BYTE "ENDLESS       ",0



msgPanelControls BYTE "Controls:            ",0  ;  spaces added
msgPanelCtrl1    BYTE "W A S D  Move        ",0  ;  spaces added
msgPanelCtrl2    BYTE "R        Random      ",0  ;  space added
msgPanelCtrl3    BYTE "Q        Quit        ",0  ;  spaces added

msgPanelObjects BYTE "Objects:",0
msgObjProf     BYTE  "P Professor",0
msgObjDest     BYTE  "H Home Door",0
msgObjCoin     BYTE  "C Coin",0
msgObjPit      BYTE  "X Pit",0
msgObjWall     BYTE  "Wall",0
msgObjBuilding BYTE  "Building",0
msgObjLake     BYTE  "Lake",0
msgObjStumble  BYTE  "Stumble",0
msgObjTrail    BYTE  "Trail",0
msgPanelFacing BYTE  "Facing: ",0
msgFaceUp      BYTE  "UP",0
msgFaceDown    BYTE  "DOWN",0
msgFaceLeft    BYTE  "LEFT",0
msgFaceRight   BYTE  "RIGHT",0
msg3DTitle     BYTE  "3D View (Pseudo)",0
msgViewClear   BYTE  "Open path",0
msgViewObject  BYTE  "Object ahead: ",0
msg3DLine1     BYTE  "+--------------------+",0
msg3DLine2     BYTE  "|\\                  /|",0
msg3DLine3     BYTE  "| \\                / |",0
msg3DLine4     BYTE  "|  \\              /  |",0
msg3DLine5     BYTE  "|   \\            /   |",0
msg3DLine6     BYTE  "|    \\          /    |",0
msg3DLine7     BYTE  "|     \\        /     |",0
msg3DLine8     BYTE  "|      \\______/      |",0
msg3DLine9     BYTE  "+--------------------+",0
viewObjChar    BYTE  ' '
viewObjDist    DWORD 0
msgSplashScore BYTE "SCORE 000100  DIZZY  COINS 00",0
msgSplashTitle BYTE "WELCOME TO DIZZY CITY",0
msgSplashSub   BYTE "ARE YOU CARRYING NOTHING?",0
msgSplashStart BYTE "Press any key to continue...",0
msgPalmTop     BYTE "  /\\  ",0
msgPalmMid     BYTE " /**\\ ",0
msgPalmTrunk   BYTE "  ||  ",0
msgGround1     BYTE "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^",0
msgGround2     BYTE "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",0
msgCloud       BYTE "  .--.    .--.  ",0
msgHero        BYTE "  P  ",0
msgCoinIcon    BYTE " (C) ",0
msgArcadeHUD1  BYTE "DIZZY WALK ARCADE",0
msgArcadeScore BYTE "  Score: ",0
msgArcadeCoins BYTE "  Coins: ",0
msgArcadeSteps BYTE "  Steps: ",0
msgDangerNear  BYTE "DANGER: PIT NEARBY!",0
msgSafeStatus  BYTE "Status: Area clear   ",0
msgEndWinTitle BYTE "=== YOU WIN! ===",0
msgEndLoseTitle BYTE "=== GAME OVER ===",0
msgEndReasonLbl BYTE "Reason: ",0
msgEndDest     BYTE "Reached home door",0
msgEndPit      BYTE "Fell into a pit",0
msgEndMax      BYTE "Step limit reached",0
msgEndQuit     BYTE "Quit by player",0
msgEndUnknown  BYTE "Unknown",0
msgEndPrompt   BYTE "Press any key to exit...",0
msgTransition  BYTE "Loading next scene...",0
msgArenaCtrl   BYTE "Controls: W/A/S/D Move  R Random  Q Quit",0
msgArenaLegend2 BYTE "Icons: P You  C Coin  H Home  X Pit  ! Stumble  ~=Lake  # Wall  B Build",0
msgStartHint   BYTE 13, 10, 13, 10  ; Yeh 5 enters screen ko neechay (center) push karenge
               BYTE "                    ===============================================", 13, 10
               BYTE "                    | WELCOME TO DIZZY WALK COAL PROJECT SPRING'26|", 13, 10
               BYTE "                    ===============================================", 13, 10
               BYTE "                                     Presented By:                 ", 13, 10,13,10
               BYTE "                                 i242005 (Ahmed Saeed)             ", 13, 10
               BYTE "                                 i242051 (Fiyaz Ahmad)             ", 13, 10
               BYTE "                                 i242018 (Afnan Abbas)             ", 13, 10
               BYTE "                    ===============================================", 13, 10
               BYTE "                    ", 13, 10
               BYTE "                              Press Any Key To Continue... ", 0
msgStartCtrl   BYTE "1:R+Limited  2:R+Endless  3:K+Limited  4:K+Endless",0

fileHandle    DWORD 0
fileBuffer    BYTE  4096 DUP(0)
fileBufLen    DWORD 0

tempStr       BYTE  32 DUP(0)
randSeed      DWORD 12345h
marqueeOffset DWORD 0

cellCharCache BYTE  0      ; FIX 6: cache for CheckCell
