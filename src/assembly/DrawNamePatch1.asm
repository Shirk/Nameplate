;
; This file is part of Nameplate.
; Copyright (C) 2024 BunnyBox Productions
;
; Nameplate is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
; published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
;
; Nameplate is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
; of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License along with Nameplate.
; If not, see <https://www.gnu.org/licenses/>.
;

BITS 32

; 10085879
LEA ECX, [ESP + 0x38] ; W THEN H
; LEA EDX, [ESP + 0x3C]
LEA EDX, [ESP + 0x6D4] ; param 3
MOV EAX, 0x12345678
; PUSH EDX
; PUSH ECX
CALL EAX
; ADD ESP, 8

FLD DWORD [ESP + 0x3C]

MOV EBP, [ESP + 0x6D0] ; param 2, s
OR ECX, 0xFFFFFFFF
MOV EDI, EBP
XOR EAX, EAX
REPNE SCASB
NOT ECX
DEC ECX
CMP ECX, 0x24

; FLAGS ARE SET AFTER CMP
; ST0 CONTAINS [ESP + 0x3C]

JMP .DONE
; ...
; 100858d1

; pad to 88 bytes
times 88 - ($-$$) db 0x90

.DONE:

