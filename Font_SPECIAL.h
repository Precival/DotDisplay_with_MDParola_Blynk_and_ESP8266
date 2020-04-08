#ifndef FONT_SPECIAL_H
#define FONTSPECIAL_H

uint8_t degC[] = {6, 3, 3, 56, 68, 68, 68};

MD_MAX72XX::fontType_t special[] PROGMEM = 
{
	0, 	// 0     
	0, 	// 1     
	0, 	// 2     
	0, 	// 3     
	0, 	// 4     
	0, 	// 5     
	0, 	// 6     
	0, 	// 7     
	0, 	// 8     
	0, 	// 9     
	0, 	// 10     
	0, 	// 11     
	0, 	// 12     
	0, 	// 13     
	0, 	// 14     
	0, 	// 15     
	0, 	// 16     
	0, 	// 17     
	0, 	// 18     
	0, 	// 19     
	0, 	// 20     
	0, 	// 21     
	0, 	// 22     
	0, 	// 23     
	0, 	// 24     
	0, 	// 25     
	0, 	// 26     
	0, 	// 27     
	0, 	// 28     
	0, 	// 29     
	0, 	// 30     
	0, 	// 31     
	1, 0, 	// 32     - 'Space'
	0, 	// 33      - '!'
	0, 	// 34      - '"'
	0, 	// 35      - '#'
	0, 	// 36      - '$'
	0, 	// 37      - '%'
	0, 	// 38      - '&'
	0, 	// 39      - '''
	0, 	// 40      - '('
	0, 	// 41      - ')'
	0, 	// 42      - '*'
	0, 	// 43      - '+'
	0, 	// 44 '
	0, 	// 45      - '-'
	1, 64, 	// 46      - '.'
	0, 	// 47      - '/'
	4, 127, 65, 65, 127, 	// 48    - '0'
	4, 0, 66, 127, 64, 	// 49       - '1'
	4, 121, 73, 73, 79, 	// 50     - '2'
	4, 99, 73, 73, 119, 	// 51     - '3'
	4, 15, 8, 8, 127, 	// 52      - '4'
	4, 79, 73, 73, 121, 	// 53     - '5'
	4, 127, 73, 73, 121, 	// 54    - '6'
	4, 3, 1, 125, 3, 	// 55       - '7'
	4, 119, 73, 73, 119, 	// 56    - '8'
	4, 79, 73, 73, 127, 	// 57     - '9'
	1, 20, 	// 58      - ':'
	8, 126, 66, 126, 0, 126, 24, 36, 66, 	// 59      - ';'
	0, 	// 60      - '<'
	0, 	// 61      - '='
	0, 	// 62      - '>'
	0, 	// 63      - '?'
	0, 	// 64      - '@'
	5, 255, 129, 129, 129, 255, 	// 65     - 'A'
	5, 255, 193, 193, 193, 255, 	// 66     - 'B'
	5, 255, 225, 225, 225, 255, 	// 67     - 'C'
	5, 255, 241, 241, 241, 255, 	// 68     - 'D'
	5, 255, 249, 249, 249, 255, 	// 69     - 'E'
	5, 255, 253, 253, 253, 255, 	// 70       - 'F'
	5, 255, 255, 255, 255, 255, 	// 71      - 'G'
	3, 0, 0, 0, 	// 72      - 'H'
	3, 128, 192, 224, 	// 73      - 'I'
	3, 224, 240, 248, 	// 74      - 'J'
	3, 248, 252, 254, 	// 75      - 'K'
	3, 255, 255, 255, 	// 76      - 'L'
	8, 254, 130, 195, 227, 243, 251, 254, 254, 	// 77      - 'M'
	8, 0, 0, 0, 0, 0, 0, 0, 0, 	// 78      - 'N'
	0, 	// 79      - 'O'
	0, 	// 80      - 'P'
	0, 	// 81      - 'Q'
	0, 	// 82      - 'R'
	0, 	// 83      - 'S'
	0, 	// 84      - 'T'
	0, 	// 85      - 'U'
	8, 129, 0, 126, 0, 126, 10, 10, 129, 	// 86      - 'V'
	8, 251, 251, 0, 255, 255, 27, 27, 27, 	// 87      - 'W'
	8, 4, 4, 255, 0, 0, 228, 228, 228, 	// 88      - 'X'
	0, 	// 89      - 'Y'
	0, 	// 90      - 'Z'
	0, 	// 91      - '['
	0, 	// 92      - '\'
	0, 	// 93      - ']'
	0, 	// 94      - '^'
	0, 	// 95      - '_'
	0, 	// 96      - '`'
	3, 0, 0, 0, 	// 97     - 'a'
	3, 128, 128, 128, 	// 98     - 'b'
	3, 192, 192, 192, 	// 99     - 'c'
	3, 224, 224, 224, 	// 100     - 'd'
	3, 240, 240, 240, 	// 101     - 'e'
	3, 248, 248, 248, 	// 102       - 'f'
	3, 252, 252, 252, 	// 103      - 'g'
	3, 254, 254, 254, 	// 104      - 'h'
	3, 255, 255, 255, 	// 105      - 'i'
	8, 127, 127, 62, 28, 20, 34, 65, 127, 	// 106 
	8, 195, 167, 159, 143, 159, 167, 195, 0, 	// 107     
	8, 195, 167, 159, 151, 159, 167, 195, 0, 	// 108     
	8, 195, 165, 159, 175, 159, 167, 195, 0, 	// 109     
	8, 195, 165, 159, 207, 159, 167, 195, 0, 	// 110     
	8, 195, 165, 159, 215, 159, 167, 195, 0, 	// 111     
	8, 195, 165, 157, 239, 159, 167, 195, 0, 	// 112     
	8, 195, 165, 157, 215, 223, 167, 195, 0, 	// 113     
	8, 195, 165, 157, 237, 223, 167, 195, 0, 	// 114     
	8, 195, 165, 221, 213, 223, 167, 195, 0, 	// 115     
	8, 195, 165, 221, 237, 221, 167, 195, 0, 	// 116     
	8, 195, 165, 221, 253, 221, 165, 195, 0, 	// 117     
	8, 195, 165, 249, 237, 221, 165, 195, 0, 	// 118     
	8, 195, 229, 217, 245, 221, 165, 195, 0, 	// 119     
	8, 195, 229, 249, 237, 217, 165, 195, 0, 	// 120     
	8, 195, 229, 249, 245, 217, 165, 195, 0, 	// 121     
	8, 195, 229, 249, 249, 249, 165, 195, 0, 	// 122     
	8, 195, 229, 249, 241, 249, 229, 195, 0, 	// 123     		
	1, 127, 	// 124     - '|'
	0, 	// 125     
	8, 153, 36, 2, 153, 153, 66, 36, 153, 	// 126     
	8, 153, 4, 66, 153, 153, 66, 36, 153, 	// 127     
	8, 137, 36, 66, 153, 153, 66, 36, 153, 	// 128     
	8, 145, 36, 66, 153, 153, 66, 36, 153, 	// 129     
	8, 153, 32, 66, 153, 153, 66, 36, 153, 	// 130     
	8, 153, 36, 64, 153, 153, 66, 36, 153, 	// 131     
	8, 153, 36, 66, 152, 153, 66, 36, 153, 	// 132     
	8, 153, 36, 66, 153, 152, 66, 36, 153, 	// 133     
	8, 153, 36, 66, 153, 153, 64, 36, 153, 	// 134     
	8, 153, 36, 66, 153, 153, 66, 32, 153, 	// 135     
	8, 153, 36, 66, 153, 153, 66, 36, 145, 	// 136     
	8, 153, 36, 66, 153, 153, 66, 36, 137, 	// 137     
	8, 153, 36, 66, 153, 153, 66, 4, 153, 	// 138     
	8, 153, 36, 66, 153, 153, 2, 36, 153, 	// 139     
	8, 153, 36, 66, 153, 25, 66, 36, 153, 	// 140     
	8, 153, 36, 66, 25, 153, 66, 36, 153, 	// 141     
	0, 	// 142     
	0, 	// 143     
	0, 	// 144     
	0, 	// 145     
	0, 	// 146     
	8, 128, 0, 0, 0, 0, 0, 0, 0, 	// 147     
	8, 224, 192, 128, 0, 0, 0, 0, 0, 	// 148     
	8, 248, 240, 224, 192, 128, 0, 0, 0, 	// 149     
	8, 254, 252, 248, 240, 224, 192, 128, 0, 	// 150     
	8, 255, 255, 254, 252, 248, 240, 224, 192, 	// 151     
	8, 255, 255, 255, 255, 254, 252, 248, 240, 	// 152     
	8, 255, 255, 255, 255, 255, 255, 255, 255, 	// 153     
	0, 	// 154     
	0, 	// 155     
	0, 	// 156     
	0, 	// 157     
	0, 	// 158     
	0, 	// 159     
	0, 	// 160     
	0, 	// 161     
	0, 	// 162     
	0, 	// 163     
	0, 	// 164     
	0, 	// 165     
	0, 	// 166     
	0, 	// 167     
	0, 	// 168     
	0, 	// 169     
	0, 	// 170     
	0, 	// 171     
	0, 	// 172     
	0, 	// 173     
	0, 	// 174     
	0, 	// 175     
	7, 7, 5, 7, 56, 68, 68, 68, 	// 176     
	0, 	// 177     
	0, 	// 178     
	0, 	// 179     
	0, 	// 180     
	0, 	// 181     
	0, 	// 182     
	0, 	// 183     
	0, 	// 184     
	0, 	// 185     
	0, 	// 186     
	0, 	// 187     
	0, 	// 188     
	0, 	// 189     
	0, 	// 190     
	0, 	// 191     
	0, 	// 192     
	0, 	// 193     
	0, 	// 194     
	0, 	// 195     
	0, 	// 196     
	0, 	// 197     
	0, 	// 198     
	0, 	// 199     
	0, 	// 200     
	0, 	// 201     
	0, 	// 202     
	0, 	// 203     
	0, 	// 204     
	0, 	// 205     
	0, 	// 206     
	0, 	// 207     
	0, 	// 208     
	0, 	// 209     
	0, 	// 210     
	0, 	// 211     
	0, 	// 212     
	0, 	// 213     
	0, 	// 214     
	0, 	// 215     
	0, 	// 216     
	0, 	// 217     
	0, 	// 218     
	0, 	// 219     
	0, 	// 220     
	0, 	// 221     
	0, 	// 222     
	0, 	// 223     
	0, 	// 224     
	0, 	// 225     
	0, 	// 226     
	0, 	// 227     
	0, 	// 228     
	0, 	// 229     
	0, 	// 230     
	0, 	// 231     
	0, 	// 232     
	0, 	// 233     
	0, 	// 234     
	0, 	// 235     
	0, 	// 236     
	0, 	// 237
	0, 	// 238
	0, 	// 239
	0, 	// 240
	0, 	// 241
	0, 	// 242
	0, 	// 243
	0, 	// 244
	0, 	// 245
	0, 	// 246
	0, 	// 247
	0, 	// 248
	0, 	// 249
	0, 	// 250
	0, 	// 251
	0, 	// 252
	0, 	// 253
	0, 	// 254
	0, 	// 255
};

#endif