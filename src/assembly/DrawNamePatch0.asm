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

; 10085185
LEA ECX, [ESP + 0x4C] ; W THEN H
; LEA EDX, [ESP + 0x50]
LEA EDX, [ESP + 0x6E0] ; param 3
MOV EAX, 0x12345678
; PUSH EDX
; PUSH ECX
CALL EAX
; ADD ESP, 8

MOV EDI, [ESP + 0x6DC] ; param 2, s
OR ECX, 0xFFFFFFFF
XOR EAX, EAX
REPNE SCASB
NOT ECX
DEC ECX
CMP ECX, 0x24
JLE .NOCLAMP
MOV ECX, 0x24
.NOCLAMP:
MOV [ESP + 0x18], ECX
JMP .DONE
; ...
; 100851EE

; pad to 105 bytes
times 105 - ($-$$) db 0x90

.DONE:
