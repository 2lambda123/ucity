;###############################################################################
;
;    µCity - City building game for Game Boy Color.
;    Copyright (c) 2017-2018 Antonio Niño Díaz (AntonioND/SkyLyrac)
;
;    This program is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;    Contact: antonio_nd@outlook.com
;
;###############################################################################

    IF !DEF(ROOM_GAME_INC)
    DEF ROOM_GAME_INC = 1

;###############################################################################

CITY_MAP_WIDTH  EQU 64
CITY_MAP_HEIGHT EQU 64

;-------------------------------------------------------------------------------

; Returns address in HL. Preserves de and bc
GET_MAP_ADDRESS : MACRO ; e = x , d = y (0 to 63)

     xor     a,a
     ld      h,d ; ha = y << 8
     srl     h
     rra
     srl     h
     rra         ; ha = ((y << 8) >> 2) = y << 6 = y * 64

     add     a,e
     ld      l,a ; hl = y * 64 + x

     ld      a,h
     add     a,CITY_MAP_TILES >> 8
     ld      h,a ; hl = base + y * 64 + x

ENDM

;-------------------------------------------------------------------------------

; This shouldn't be needed, but if removed the assembler will complain when
; getting the BANK() of a label that hasn't been used before in that file.
BANK_CITY_MAP_TILES   EQU 1
BANK_CITY_MAP_ATTR    EQU 2
BANK_CITY_MAP_TYPE    EQU 3
BANK_CITY_MAP_TRAFFIC EQU 4
BANK_CITY_MAP_FLAGS   EQU 5 ; How 'happy' each tile is, automatic build commands
BANK_SCRATCH_RAM      EQU 6 ; For temporary calculations (simulation)
BANK_SCRATCH_RAM_2    EQU 7 ; For temporary calculations (APA, simulation)

;-------------------------------------------------------------------------------

TILE_OK_POWER_BIT           EQU 0
TILE_OK_SERVICES_BIT        EQU 1 ; Hospitals, police and firemen
TILE_OK_EDUCATION_BIT       EQU 2
TILE_OK_POLLUTION_BIT       EQU 3 ; Set to 1 if valid pollution level
TILE_OK_TRAFFIC_BIT         EQU 4 ; - Roads: Traffic lower than threshold.
                                  ; - Buildings: People from this tile could
                                  ;   reach a valid destination.
TILE_BUILD_REQUESTED_BIT    EQU 5
TILE_DEMOLISH_REQUESTED_BIT EQU 6
TILE_unused_BIT             EQU 7

TILE_OK_POWER       EQU (1<<TILE_OK_POWER_BIT)
TILE_OK_SERVICES    EQU (1<<TILE_OK_SERVICES_BIT)
TILE_OK_EDUCATION   EQU (1<<TILE_OK_EDUCATION_BIT)
TILE_OK_POLLUTION   EQU (1<<TILE_OK_POLLUTION_BIT)
TILE_OK_TRAFFIC     EQU (1<<TILE_OK_TRAFFIC_BIT)

TILE_OK_MASK                EQU %00011111

;-------------------------------------------------------------------------------

DISASTER_TYPE_NONE     EQU 0
DISASTER_TYPE_FIRE     EQU 1
DISASTER_TYPE_MELTDOWN EQU 2

;-------------------------------------------------------------------------------

GAME_STATE_WATCH            EQU 0
GAME_STATE_EDIT             EQU 1
GAME_STATE_WATCH_FAST_MOVE  EQU 2
GAME_STATE_SELECT_BUILDING  EQU 3
GAME_STATE_PAUSE_MENU       EQU 4
GAME_STATE_SHOW_MESSAGE     EQU 5

;-------------------------------------------------------------------------------

CLASS_VILLAGE    EQU 0
CLASS_TOWN       EQU 1
CLASS_CITY       EQU 2
CLASS_METROPOLIS EQU 3
CLASS_CAPITAL    EQU 4

;-------------------------------------------------------------------------------

; Item type
; ---------

TYPE_HAS_ROAD_BIT  EQU 7
TYPE_HAS_TRAIN_BIT EQU 6
TYPE_HAS_POWER_BIT EQU 5

TYPE_HAS_ROAD   EQU (1<<TYPE_HAS_ROAD_BIT)
TYPE_HAS_TRAIN  EQU (1<<TYPE_HAS_TRAIN_BIT)
TYPE_HAS_POWER  EQU (1<<TYPE_HAS_POWER_BIT)

TYPE_MASK       EQU %00011111
TYPE_FLAGS_MASK EQU %11100000

; TYPE_FIELD must be 0 always so that TYPE_HAS_ROAD, TYPE_HAS_TRAIN and
; TYPE_HAS_POWER are always considered to be in TYPE_FIELD. The exceptions are
; bridges, that are a combination of ``TYPE_WATER`` and the corresponding flag.
TYPE_FIELD              EQU 0 ; This one must be 0.
TYPE_FOREST             EQU 1
TYPE_WATER              EQU 2
TYPE_RESIDENTIAL        EQU 3
TYPE_INDUSTRIAL         EQU 4
TYPE_COMMERCIAL         EQU 5
TYPE_POLICE_DEPT        EQU 6
TYPE_FIRE_DEPT          EQU 7
TYPE_HOSPITAL           EQU 8
TYPE_PARK               EQU 9
TYPE_STADIUM            EQU 10
TYPE_SCHOOL             EQU 11
TYPE_HIGH_SCHOOL        EQU 12
TYPE_UNIVERSITY         EQU 13
TYPE_MUSEUM             EQU 14
TYPE_LIBRARY            EQU 15
TYPE_AIRPORT            EQU 16
TYPE_PORT               EQU 17
TYPE_DOCK               EQU 18
TYPE_POWER_PLANT        EQU 19
TYPE_FIRE               EQU 20
TYPE_RADIATION          EQU 21

TYPE_NUMBER             EQU 32 ; Max. number. Upper 3 bits used as flags

;###############################################################################

    ENDC ; ROOM_GAME_INC

;###############################################################################
