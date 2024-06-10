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

.ENTRY:

; ECX, EDX, EAX are already spoilable by the original
MOV ECX, ESI
MOV EAX, 0x12345678
CALL EAX
TEST AL, AL
JNZ .DONE
JZ .EXIT

; pad to 40 bytes
.PAD0:
times 40 - ($ - $$) db 0x90

; cleanup - don't know if it needs to be called but...
.EXIT:
MOV EDX, [ESI]
MOV ECX, ESI
CALL [EDX + 0xCC]

; THIS MUST REMAIN HERE AT 50 BYTES, BECAUSE SURROUNDING CODE CAN JUMP HERE!
POP EDI
POP ESI
ADD ESP, 0x118
RET

; pad to 131 bytes
times 131 - ($-$$) db 0x90

.DONE:
