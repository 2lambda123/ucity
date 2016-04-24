;###############################################################################
;
;    BitCity - City building game for Game Boy Color.
;    Copyright (C) 2016 Antonio Nino Diaz (AntonioND/SkyLyrac)
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

    INCLUDE "hardware.inc"
    INCLUDE "engine.inc"

;-------------------------------------------------------------------------------

    INCLUDE "room_game.inc"
    INCLUDE "text.inc"
    INCLUDE "building_density.inc"

;###############################################################################

    SECTION "Minimap Population Density Map Functions",ROMX

;-------------------------------------------------------------------------------

MINIMAP_TILE_COLORS:
    DB 0,0,0,0
    DB 0,1,1,0
    DB 1,1,1,1
    DB 1,2,2,1
    DB 2,2,2,2
    DB 2,3,3,2
    DB 3,3,3,3
    DB 3,3,3,3

MINIMAP_POPULATION_DENSITY_MAP_PALETTE:
    DW (31<<10)|(31<<5)|(31<<0), (0<<10)|(31<<5)|(0<<0)
    DW (0<<10)|(31<<5)|(31<<0), (0<<10)|(0<<5)|(31<<0)

MINIMAP_POPULATION_DENSITY_MAP_TITLE:
    DB "Population Density",0

MinimapDrawPopulationDensityMap::

    ; Draw map
    ; --------

    LONG_CALL   APA_PixelStreamStart

    ld      hl,CITY_MAP_TILES ; Base address of the map!

    ld      d,0 ; d = y
.loopy:

        ld      e,0 ; e = x
.loopx:

        push    de ; (*)
        push    hl

            call    CityMapGetTileAtAddress ; hl = address, returns de = tile

IF CITY_TILE_DENSITY_ELEMENT_SIZE != 2
    FAIL "Fix this!"
ENDC

            ld      hl, CITY_TILE_DENSITY
            add     hl,de
            add     hl,de ; Get first elemennt (population density)

            ld      a,BANK(CITY_TILE_DENSITY)
            ld      [rSVBK],a

            ld      a,[hl]
            cp      a,MAX_DISPLAYABLE_POPULATION_DENSITY+1 ; Saturate
            jr      c,.not_overflow
            ld      a,MAX_DISPLAYABLE_POPULATION_DENSITY
.not_overflow:

IF MAX_DISPLAYABLE_POPULATION_DENSITY != 15
    FAIL "Fix this!"
ENDC
            sra     a ; From 4 bits to 3 (15 -> 7)

            ld      de,MINIMAP_TILE_COLORS
            ld      l,a
            ld      h,0
            add     hl,hl
            add     hl,hl ; a *= 4
            add     hl,de

            ld      a,[hl+]
            ld      b,[hl]
            inc     hl
            ld      c,[hl]
            inc     hl
            ld      d,[hl]

            call    APA_SetColors ; a,b,c,d = color (0 to 3)
            LONG_CALL   APA_PixelStreamPlot2x2

        pop     hl
        pop     de ; (*)

        inc     hl

        inc     e
        bit     6,e
        jp      z,.loopx

    inc     d
    bit     6,d
    jp      z,.loopy

    ; Set White
    call    MinimapSetDefaultPalette

    ; Refresh screen with backbuffer data
    call    APA_BufferUpdate

    ; Load palette
    ld      hl,MINIMAP_POPULATION_DENSITY_MAP_PALETTE
    call    APA_LoadPalette

    ; Draw title
    ld      hl,MINIMAP_POPULATION_DENSITY_MAP_TITLE
    call    RoomMinimapDrawTitle

    ret

;###############################################################################
