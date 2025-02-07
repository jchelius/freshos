/* ISR common routine */
.extern isr_handler
isr_common:
	pusha
	pushw %ds
	pushw %es
	pushw %fs
	pushw %gs
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	pushl %esp
	cld
	call isr_handler
	popl %eax
	popw %gs
	popw %fs
	popw %es
	popw %ds
	popa
	addl $8, %esp
	iret

/* ISR definitions */
isr0:
	pushl $0
	pushl $0
	jmp isr_common
isr1:
	pushl $0
	pushl $1
	jmp isr_common
isr2:
	pushl $0
	pushl $2
	jmp isr_common
isr3:
	pushl $0
	pushl $3
	jmp isr_common
isr4:
	pushl $0
	pushl $4
	jmp isr_common
isr5:
	pushl $0
	pushl $5
	jmp isr_common
isr6:
	pushl $0
	pushl $6
	jmp isr_common
isr7:
	pushl $0
	pushl $7
	jmp isr_common
isr8:
	pushl $8
	jmp isr_common
isr9:
	pushl $0
	pushl $9
	jmp isr_common
isr10:
	pushl $10
	jmp isr_common
isr11:
	pushl $11
	jmp isr_common
isr12:
	pushl $12
	jmp isr_common
isr13:
	pushl $13
	jmp isr_common
isr14:
	pushl $14
	jmp isr_common
isr15:
	pushl $0
	pushl $15
	jmp isr_common
isr16:
	pushl $0
	pushl $16
	jmp isr_common
isr17:
	pushl $0
	pushl $17
	jmp isr_common
isr18:
	pushl $0
	pushl $18
	jmp isr_common
isr19:
	pushl $0
	pushl $19
	jmp isr_common
isr20:
	pushl $0
	pushl $20
	jmp isr_common
isr21:
	pushl $0
	pushl $21
	jmp isr_common
isr22:
	pushl $0
	pushl $22
	jmp isr_common
isr23:
	pushl $0
	pushl $23
	jmp isr_common
isr24:
	pushl $0
	pushl $24
	jmp isr_common
isr25:
	pushl $0
	pushl $25
	jmp isr_common
isr26:
	pushl $0
	pushl $26
	jmp isr_common
isr27:
	pushl $0
	pushl $27
	jmp isr_common
isr28:
	pushl $0
	pushl $28
	jmp isr_common
isr29:
	pushl $0
	pushl $29
	jmp isr_common
isr30:
	pushl $0
	pushl $30
	jmp isr_common
isr31:
	pushl $0
	pushl $31
	jmp isr_common
isr32:
	pushl $0
	pushl $32
	jmp isr_common
isr33:
	pushl $0
	pushl $33
	jmp isr_common
isr34:
	pushl $0
	pushl $34
	jmp isr_common
isr35:
	pushl $0
	pushl $35
	jmp isr_common
isr36:
	pushl $0
	pushl $36
	jmp isr_common
isr37:
	pushl $0
	pushl $37
	jmp isr_common
isr38:
	pushl $0
	pushl $38
	jmp isr_common
isr39:
	pushl $0
	pushl $39
	jmp isr_common
isr40:
	pushl $0
	pushl $40
	jmp isr_common
isr41:
	pushl $0
	pushl $41
	jmp isr_common
isr42:
	pushl $0
	pushl $42
	jmp isr_common
isr43:
	pushl $0
	pushl $43
	jmp isr_common
isr44:
	pushl $0
	pushl $44
	jmp isr_common
isr45:
	pushl $0
	pushl $45
	jmp isr_common
isr46:
	pushl $0
	pushl $46
	jmp isr_common
isr47:
	pushl $0
	pushl $47
	jmp isr_common
isr48:
	pushl $0
	pushl $48
	jmp isr_common
isr49:
	pushl $0
	pushl $49
	jmp isr_common
isr50:
	pushl $0
	pushl $50
	jmp isr_common
isr51:
	pushl $0
	pushl $51
	jmp isr_common
isr52:
	pushl $0
	pushl $52
	jmp isr_common
isr53:
	pushl $0
	pushl $53
	jmp isr_common
isr54:
	pushl $0
	pushl $54
	jmp isr_common
isr55:
	pushl $0
	pushl $55
	jmp isr_common
isr56:
	pushl $0
	pushl $56
	jmp isr_common
isr57:
	pushl $0
	pushl $57
	jmp isr_common
isr58:
	pushl $0
	pushl $58
	jmp isr_common
isr59:
	pushl $0
	pushl $59
	jmp isr_common
isr60:
	pushl $0
	pushl $60
	jmp isr_common
isr61:
	pushl $0
	pushl $61
	jmp isr_common
isr62:
	pushl $0
	pushl $62
	jmp isr_common
isr63:
	pushl $0
	pushl $63
	jmp isr_common
isr64:
	pushl $0
	pushl $64
	jmp isr_common
isr65:
	pushl $0
	pushl $65
	jmp isr_common
isr66:
	pushl $0
	pushl $66
	jmp isr_common
isr67:
	pushl $0
	pushl $67
	jmp isr_common
isr68:
	pushl $0
	pushl $68
	jmp isr_common
isr69:
	pushl $0
	pushl $69
	jmp isr_common
isr70:
	pushl $0
	pushl $70
	jmp isr_common
isr71:
	pushl $0
	pushl $71
	jmp isr_common
isr72:
	pushl $0
	pushl $72
	jmp isr_common
isr73:
	pushl $0
	pushl $73
	jmp isr_common
isr74:
	pushl $0
	pushl $74
	jmp isr_common
isr75:
	pushl $0
	pushl $75
	jmp isr_common
isr76:
	pushl $0
	pushl $76
	jmp isr_common
isr77:
	pushl $0
	pushl $77
	jmp isr_common
isr78:
	pushl $0
	pushl $78
	jmp isr_common
isr79:
	pushl $0
	pushl $79
	jmp isr_common
isr80:
	pushl $0
	pushl $80
	jmp isr_common
isr81:
	pushl $0
	pushl $81
	jmp isr_common
isr82:
	pushl $0
	pushl $82
	jmp isr_common
isr83:
	pushl $0
	pushl $83
	jmp isr_common
isr84:
	pushl $0
	pushl $84
	jmp isr_common
isr85:
	pushl $0
	pushl $85
	jmp isr_common
isr86:
	pushl $0
	pushl $86
	jmp isr_common
isr87:
	pushl $0
	pushl $87
	jmp isr_common
isr88:
	pushl $0
	pushl $88
	jmp isr_common
isr89:
	pushl $0
	pushl $89
	jmp isr_common
isr90:
	pushl $0
	pushl $90
	jmp isr_common
isr91:
	pushl $0
	pushl $91
	jmp isr_common
isr92:
	pushl $0
	pushl $92
	jmp isr_common
isr93:
	pushl $0
	pushl $93
	jmp isr_common
isr94:
	pushl $0
	pushl $94
	jmp isr_common
isr95:
	pushl $0
	pushl $95
	jmp isr_common
isr96:
	pushl $0
	pushl $96
	jmp isr_common
isr97:
	pushl $0
	pushl $97
	jmp isr_common
isr98:
	pushl $0
	pushl $98
	jmp isr_common
isr99:
	pushl $0
	pushl $99
	jmp isr_common
isr100:
	pushl $0
	pushl $100
	jmp isr_common
isr101:
	pushl $0
	pushl $101
	jmp isr_common
isr102:
	pushl $0
	pushl $102
	jmp isr_common
isr103:
	pushl $0
	pushl $103
	jmp isr_common
isr104:
	pushl $0
	pushl $104
	jmp isr_common
isr105:
	pushl $0
	pushl $105
	jmp isr_common
isr106:
	pushl $0
	pushl $106
	jmp isr_common
isr107:
	pushl $0
	pushl $107
	jmp isr_common
isr108:
	pushl $0
	pushl $108
	jmp isr_common
isr109:
	pushl $0
	pushl $109
	jmp isr_common
isr110:
	pushl $0
	pushl $110
	jmp isr_common
isr111:
	pushl $0
	pushl $111
	jmp isr_common
isr112:
	pushl $0
	pushl $112
	jmp isr_common
isr113:
	pushl $0
	pushl $113
	jmp isr_common
isr114:
	pushl $0
	pushl $114
	jmp isr_common
isr115:
	pushl $0
	pushl $115
	jmp isr_common
isr116:
	pushl $0
	pushl $116
	jmp isr_common
isr117:
	pushl $0
	pushl $117
	jmp isr_common
isr118:
	pushl $0
	pushl $118
	jmp isr_common
isr119:
	pushl $0
	pushl $119
	jmp isr_common
isr120:
	pushl $0
	pushl $120
	jmp isr_common
isr121:
	pushl $0
	pushl $121
	jmp isr_common
isr122:
	pushl $0
	pushl $122
	jmp isr_common
isr123:
	pushl $0
	pushl $123
	jmp isr_common
isr124:
	pushl $0
	pushl $124
	jmp isr_common
isr125:
	pushl $0
	pushl $125
	jmp isr_common
isr126:
	pushl $0
	pushl $126
	jmp isr_common
isr127:
	pushl $0
	pushl $127
	jmp isr_common
isr128:
	pushl $0
	pushl $128
	jmp isr_common
isr129:
	pushl $0
	pushl $129
	jmp isr_common
isr130:
	pushl $0
	pushl $130
	jmp isr_common
isr131:
	pushl $0
	pushl $131
	jmp isr_common
isr132:
	pushl $0
	pushl $132
	jmp isr_common
isr133:
	pushl $0
	pushl $133
	jmp isr_common
isr134:
	pushl $0
	pushl $134
	jmp isr_common
isr135:
	pushl $0
	pushl $135
	jmp isr_common
isr136:
	pushl $0
	pushl $136
	jmp isr_common
isr137:
	pushl $0
	pushl $137
	jmp isr_common
isr138:
	pushl $0
	pushl $138
	jmp isr_common
isr139:
	pushl $0
	pushl $139
	jmp isr_common
isr140:
	pushl $0
	pushl $140
	jmp isr_common
isr141:
	pushl $0
	pushl $141
	jmp isr_common
isr142:
	pushl $0
	pushl $142
	jmp isr_common
isr143:
	pushl $0
	pushl $143
	jmp isr_common
isr144:
	pushl $0
	pushl $144
	jmp isr_common
isr145:
	pushl $0
	pushl $145
	jmp isr_common
isr146:
	pushl $0
	pushl $146
	jmp isr_common
isr147:
	pushl $0
	pushl $147
	jmp isr_common
isr148:
	pushl $0
	pushl $148
	jmp isr_common
isr149:
	pushl $0
	pushl $149
	jmp isr_common
isr150:
	pushl $0
	pushl $150
	jmp isr_common
isr151:
	pushl $0
	pushl $151
	jmp isr_common
isr152:
	pushl $0
	pushl $152
	jmp isr_common
isr153:
	pushl $0
	pushl $153
	jmp isr_common
isr154:
	pushl $0
	pushl $154
	jmp isr_common
isr155:
	pushl $0
	pushl $155
	jmp isr_common
isr156:
	pushl $0
	pushl $156
	jmp isr_common
isr157:
	pushl $0
	pushl $157
	jmp isr_common
isr158:
	pushl $0
	pushl $158
	jmp isr_common
isr159:
	pushl $0
	pushl $159
	jmp isr_common
isr160:
	pushl $0
	pushl $160
	jmp isr_common
isr161:
	pushl $0
	pushl $161
	jmp isr_common
isr162:
	pushl $0
	pushl $162
	jmp isr_common
isr163:
	pushl $0
	pushl $163
	jmp isr_common
isr164:
	pushl $0
	pushl $164
	jmp isr_common
isr165:
	pushl $0
	pushl $165
	jmp isr_common
isr166:
	pushl $0
	pushl $166
	jmp isr_common
isr167:
	pushl $0
	pushl $167
	jmp isr_common
isr168:
	pushl $0
	pushl $168
	jmp isr_common
isr169:
	pushl $0
	pushl $169
	jmp isr_common
isr170:
	pushl $0
	pushl $170
	jmp isr_common
isr171:
	pushl $0
	pushl $171
	jmp isr_common
isr172:
	pushl $0
	pushl $172
	jmp isr_common
isr173:
	pushl $0
	pushl $173
	jmp isr_common
isr174:
	pushl $0
	pushl $174
	jmp isr_common
isr175:
	pushl $0
	pushl $175
	jmp isr_common
isr176:
	pushl $0
	pushl $176
	jmp isr_common
isr177:
	pushl $0
	pushl $177
	jmp isr_common
isr178:
	pushl $0
	pushl $178
	jmp isr_common
isr179:
	pushl $0
	pushl $179
	jmp isr_common
isr180:
	pushl $0
	pushl $180
	jmp isr_common
isr181:
	pushl $0
	pushl $181
	jmp isr_common
isr182:
	pushl $0
	pushl $182
	jmp isr_common
isr183:
	pushl $0
	pushl $183
	jmp isr_common
isr184:
	pushl $0
	pushl $184
	jmp isr_common
isr185:
	pushl $0
	pushl $185
	jmp isr_common
isr186:
	pushl $0
	pushl $186
	jmp isr_common
isr187:
	pushl $0
	pushl $187
	jmp isr_common
isr188:
	pushl $0
	pushl $188
	jmp isr_common
isr189:
	pushl $0
	pushl $189
	jmp isr_common
isr190:
	pushl $0
	pushl $190
	jmp isr_common
isr191:
	pushl $0
	pushl $191
	jmp isr_common
isr192:
	pushl $0
	pushl $192
	jmp isr_common
isr193:
	pushl $0
	pushl $193
	jmp isr_common
isr194:
	pushl $0
	pushl $194
	jmp isr_common
isr195:
	pushl $0
	pushl $195
	jmp isr_common
isr196:
	pushl $0
	pushl $196
	jmp isr_common
isr197:
	pushl $0
	pushl $197
	jmp isr_common
isr198:
	pushl $0
	pushl $198
	jmp isr_common
isr199:
	pushl $0
	pushl $199
	jmp isr_common
isr200:
	pushl $0
	pushl $200
	jmp isr_common
isr201:
	pushl $0
	pushl $201
	jmp isr_common
isr202:
	pushl $0
	pushl $202
	jmp isr_common
isr203:
	pushl $0
	pushl $203
	jmp isr_common
isr204:
	pushl $0
	pushl $204
	jmp isr_common
isr205:
	pushl $0
	pushl $205
	jmp isr_common
isr206:
	pushl $0
	pushl $206
	jmp isr_common
isr207:
	pushl $0
	pushl $207
	jmp isr_common
isr208:
	pushl $0
	pushl $208
	jmp isr_common
isr209:
	pushl $0
	pushl $209
	jmp isr_common
isr210:
	pushl $0
	pushl $210
	jmp isr_common
isr211:
	pushl $0
	pushl $211
	jmp isr_common
isr212:
	pushl $0
	pushl $212
	jmp isr_common
isr213:
	pushl $0
	pushl $213
	jmp isr_common
isr214:
	pushl $0
	pushl $214
	jmp isr_common
isr215:
	pushl $0
	pushl $215
	jmp isr_common
isr216:
	pushl $0
	pushl $216
	jmp isr_common
isr217:
	pushl $0
	pushl $217
	jmp isr_common
isr218:
	pushl $0
	pushl $218
	jmp isr_common
isr219:
	pushl $0
	pushl $219
	jmp isr_common
isr220:
	pushl $0
	pushl $220
	jmp isr_common
isr221:
	pushl $0
	pushl $221
	jmp isr_common
isr222:
	pushl $0
	pushl $222
	jmp isr_common
isr223:
	pushl $0
	pushl $223
	jmp isr_common
isr224:
	pushl $0
	pushl $224
	jmp isr_common
isr225:
	pushl $0
	pushl $225
	jmp isr_common
isr226:
	pushl $0
	pushl $226
	jmp isr_common
isr227:
	pushl $0
	pushl $227
	jmp isr_common
isr228:
	pushl $0
	pushl $228
	jmp isr_common
isr229:
	pushl $0
	pushl $229
	jmp isr_common
isr230:
	pushl $0
	pushl $230
	jmp isr_common
isr231:
	pushl $0
	pushl $231
	jmp isr_common
isr232:
	pushl $0
	pushl $232
	jmp isr_common
isr233:
	pushl $0
	pushl $233
	jmp isr_common
isr234:
	pushl $0
	pushl $234
	jmp isr_common
isr235:
	pushl $0
	pushl $235
	jmp isr_common
isr236:
	pushl $0
	pushl $236
	jmp isr_common
isr237:
	pushl $0
	pushl $237
	jmp isr_common
isr238:
	pushl $0
	pushl $238
	jmp isr_common
isr239:
	pushl $0
	pushl $239
	jmp isr_common
isr240:
	pushl $0
	pushl $240
	jmp isr_common
isr241:
	pushl $0
	pushl $241
	jmp isr_common
isr242:
	pushl $0
	pushl $242
	jmp isr_common
isr243:
	pushl $0
	pushl $243
	jmp isr_common
isr244:
	pushl $0
	pushl $244
	jmp isr_common
isr245:
	pushl $0
	pushl $245
	jmp isr_common
isr246:
	pushl $0
	pushl $246
	jmp isr_common
isr247:
	pushl $0
	pushl $247
	jmp isr_common
isr248:
	pushl $0
	pushl $248
	jmp isr_common
isr249:
	pushl $0
	pushl $249
	jmp isr_common
isr250:
	pushl $0
	pushl $250
	jmp isr_common
isr251:
	pushl $0
	pushl $251
	jmp isr_common
isr252:
	pushl $0
	pushl $252
	jmp isr_common
isr253:
	pushl $0
	pushl $253
	jmp isr_common
isr254:
	pushl $0
	pushl $254
	jmp isr_common
isr255:
	pushl $0
	pushl $255
	jmp isr_common

.data
.global isrs
isrs:
	.long isr0
	.long isr1
	.long isr2
	.long isr3
	.long isr4
	.long isr5
	.long isr6
	.long isr7
	.long isr8
	.long isr9
	.long isr10
	.long isr11
	.long isr12
	.long isr13
	.long isr14
	.long isr15
	.long isr16
	.long isr17
	.long isr18
	.long isr19
	.long isr20
	.long isr21
	.long isr22
	.long isr23
	.long isr24
	.long isr25
	.long isr26
	.long isr27
	.long isr28
	.long isr29
	.long isr30
	.long isr31
	.long isr32
	.long isr33
	.long isr34
	.long isr35
	.long isr36
	.long isr37
	.long isr38
	.long isr39
	.long isr40
	.long isr41
	.long isr42
	.long isr43
	.long isr44
	.long isr45
	.long isr46
	.long isr47
	.long isr48
	.long isr49
	.long isr50
	.long isr51
	.long isr52
	.long isr53
	.long isr54
	.long isr55
	.long isr56
	.long isr57
	.long isr58
	.long isr59
	.long isr60
	.long isr61
	.long isr62
	.long isr63
	.long isr64
	.long isr65
	.long isr66
	.long isr67
	.long isr68
	.long isr69
	.long isr70
	.long isr71
	.long isr72
	.long isr73
	.long isr74
	.long isr75
	.long isr76
	.long isr77
	.long isr78
	.long isr79
	.long isr80
	.long isr81
	.long isr82
	.long isr83
	.long isr84
	.long isr85
	.long isr86
	.long isr87
	.long isr88
	.long isr89
	.long isr90
	.long isr91
	.long isr92
	.long isr93
	.long isr94
	.long isr95
	.long isr96
	.long isr97
	.long isr98
	.long isr99
	.long isr100
	.long isr101
	.long isr102
	.long isr103
	.long isr104
	.long isr105
	.long isr106
	.long isr107
	.long isr108
	.long isr109
	.long isr110
	.long isr111
	.long isr112
	.long isr113
	.long isr114
	.long isr115
	.long isr116
	.long isr117
	.long isr118
	.long isr119
	.long isr120
	.long isr121
	.long isr122
	.long isr123
	.long isr124
	.long isr125
	.long isr126
	.long isr127
	.long isr128
	.long isr129
	.long isr130
	.long isr131
	.long isr132
	.long isr133
	.long isr134
	.long isr135
	.long isr136
	.long isr137
	.long isr138
	.long isr139
	.long isr140
	.long isr141
	.long isr142
	.long isr143
	.long isr144
	.long isr145
	.long isr146
	.long isr147
	.long isr148
	.long isr149
	.long isr150
	.long isr151
	.long isr152
	.long isr153
	.long isr154
	.long isr155
	.long isr156
	.long isr157
	.long isr158
	.long isr159
	.long isr160
	.long isr161
	.long isr162
	.long isr163
	.long isr164
	.long isr165
	.long isr166
	.long isr167
	.long isr168
	.long isr169
	.long isr170
	.long isr171
	.long isr172
	.long isr173
	.long isr174
	.long isr175
	.long isr176
	.long isr177
	.long isr178
	.long isr179
	.long isr180
	.long isr181
	.long isr182
	.long isr183
	.long isr184
	.long isr185
	.long isr186
	.long isr187
	.long isr188
	.long isr189
	.long isr190
	.long isr191
	.long isr192
	.long isr193
	.long isr194
	.long isr195
	.long isr196
	.long isr197
	.long isr198
	.long isr199
	.long isr200
	.long isr201
	.long isr202
	.long isr203
	.long isr204
	.long isr205
	.long isr206
	.long isr207
	.long isr208
	.long isr209
	.long isr210
	.long isr211
	.long isr212
	.long isr213
	.long isr214
	.long isr215
	.long isr216
	.long isr217
	.long isr218
	.long isr219
	.long isr220
	.long isr221
	.long isr222
	.long isr223
	.long isr224
	.long isr225
	.long isr226
	.long isr227
	.long isr228
	.long isr229
	.long isr230
	.long isr231
	.long isr232
	.long isr233
	.long isr234
	.long isr235
	.long isr236
	.long isr237
	.long isr238
	.long isr239
	.long isr240
	.long isr241
	.long isr242
	.long isr243
	.long isr244
	.long isr245
	.long isr246
	.long isr247
	.long isr248
	.long isr249
	.long isr250
	.long isr251
	.long isr252
	.long isr253
	.long isr254
	.long isr255
