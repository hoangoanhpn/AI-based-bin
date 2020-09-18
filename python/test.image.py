#!/usr/bin/python
#11 179 209 178 122 115 145 156 140 3 156 240 43 213 178 89 142 122 212 209 146 132 16 112 71 165 56 146 121 52 126 20 133 136 233 80 202 196 138 166 219 137 36 212 18 244 224 213 57 9 29 234 148 206 121 230 188 246 242 238 194 230 27 232 150 52 242 131 60 150 251 153 115 30 65 56 220 48 122 129 129 248 115 212 224 192 126 111 81 94 235 225 27 231 212 52 11 73 229 255 0 91 180 163 28 228 146 164 140 159 115 128 127 26 219 90 120 201 165 60 84 109 210 160 115 197 87 144 213 73 143 21 74 99 84 39 110 43 199 72 42 229 125 234 237 180 101 136 32 113 94 219 224 235 39 177 240 253 164 114 46 217 29 124 198 30 155 185 3 216 227 25 30 181 208 70 50 120 21 105 32 98 51 138 108 145 149 28 138 169 33 0 213 103 97 85 228 62 245 82 86 245 170 51 53 103 206 221 107 203 229 131 23 70 53 249 200 56 227 214 189 107 225 87 135 116 251 109 66 27 255 0 17 89 249 214 187 78 216 79 94 71 13 140 143 242 107 184 212 13 179 222 204 108 148 173 185 118 49 171 117 11 158 1 252 49 82 217 196 29 212 87 163 120 127 195 182 210 217 44 215 32 157 221 20 113 92 207 139 244 216 236 111 30 56 142 83 168 207 189 114 19 117 53 81 207 21 19 12 138 167 50 145 212 86 124 228 115 89 179 158 181 169 167 232 186 101 163 249 150 182 145 43 244 39 25 63 173 109 199 33 3 25 171 17 177 207 90 215 211 93 4 138 25 213 113 253 227 138 244 13 59 93 107 93 56 5 217 34 142 1 7 32 26 227 117 253 73 238 231 121 37 108 179 28 154 230 165 148 22 60 213 105 31 154 189 162 125 153 245 43 116 189 109 182 237 34 137 27 56 218 164 242 127 42 216 248 133 109 162 199 117 26 232 133 54 170 126 240 198 229 144 156 246 39 57 227 211 143 214 188 230 232 224 145 89 147 191 6 186 64 193 130 186 183 4 2 172 15 80 122 31 113 86 98 110 6 121 53 114 38 226 172 36 152 20 225 46 222 84 149 111 80 113 85 238 46 29 129 203 146 127 218 230 168 188 152 53 23 156 132 242 113 245 164 105 64 229 91 63 67 85

byte_arr = [ 11, 179, 209, 178, 122, 115, 145, 156, 140, 3, 156, 240, 43, 213, 178, 89, 142, 122, 212, 209, 146, 132, 16, 112, 71, 165, 56, 146, 121, 52, 126, 20, 133, 136, 233, 80, 202, 196, 138, 166, 219, 137, 36, 212, 18, 244, 224, 213, 57, 9, 29, 234, 148, 206, 121, 230, 188, 246, 242, 238, 194, 230, 27, 232, 150, 52, 242, 131, 60, 150, 251, 153, 115, 30, 65, 56, 220, 48, 122, 129, 129, 248, 115, 212, 224, 192, 126, 111, 81, 94, 235, 225, 27, 231, 212, 52, 11, 73, 229, 255, 0, 91, 180, 163, 28, 228, 146, 164, 140, 159, 115, 128, 127, 26, 219, 90, 120, 201, 165, 60, 84, 109, 210, 160, 115, 197, 87, 144, 213, 73, 143, 21, 74, 99, 84, 39, 110, 43, 199, 72, 42, 229, 125, 234, 237, 180, 101, 136, 32, 113, 94, 219, 224, 235, 39, 177, 240, 253, 164, 114, 46, 217, 29, 124, 198, 30, 155, 185, 3, 216, 227, 25, 30, 181, 208, 70, 50, 120, 21, 105, 32, 98, 51, 138, 108, 145, 149, 28, 138, 169, 33, 0, 213, 103, 97, 85, 228, 62, 245, 82, 86, 245, 170, 51, 53, 103, 206, 221, 107, 203, 229, 131, 23, 70, 53, 249, 200, 56, 227, 214, 189, 107, 225, 87, 135, 116, 251, 109, 66, 27, 255, 0, 17, 89, 249, 214, 187, 78, 216, 79, 94, 71, 13, 140, 143, 242, 107, 184, 212, 13, 179, 222, 204, 108, 148, 173, 185, 118, 49, 171, 117, 11, 158, 1, 252, 49, 82, 217, 196, 29, 212, 87, 163, 120, 127, 195, 182, 210, 217, 44, 215, 32, 157, 221, 20, 113, 92, 207, 139, 244, 216, 236, 111, 30, 56, 142, 83, 168, 207, 189, 114, 19, 117, 53, 81, 207, 21, 19, 12, 138, 167, 50, 145, 212, 86, 124, 228, 115, 89, 179, 158, 181, 169, 167, 232, 186, 101, 163, 249, 150, 182, 145, 43, 244, 39, 25, 63, 173, 109, 199, 33, 3, 25, 171, 17, 177, 207, 90, 215, 211, 93, 4, 138, 25, 213, 113, 253, 227, 138, 244, 13, 59, 93, 107, 93, 56, 5, 217, 34, 142, 1, 7, 32, 26, 227, 117, 253, 73, 238, 231, 121, 37, 108, 179, 28, 154, 230, 165, 148, 22, 60, 213, 105, 31, 154, 189, 162, 125, 153, 245, 43, 116, 189, 109, 182, 237, 34, 137, 27, 56, 218, 164, 242, 127, 42, 216, 248, 133, 109, 162, 199, 117, 26, 232, 133, 54, 170, 126, 240, 198, 229, 144, 156, 246, 39, 57, 227, 211, 143, 214, 188, 230, 232, 224, 145, 89, 147, 191, 6, 186, 64, 193, 130, 186, 183, 4, 2, 172, 15, 80, 122, 31, 113, 86, 98, 110, 6, 121, 53, 114, 38, 226, 172, 36, 152, 20, 225, 46, 222, 84, 149, 111, 80, 113, 85, 238, 46, 29, 129, 203, 146, 127, 218, 230, 168, 188, 152, 53, 23, 156, 132, 242, 113, 245, 164, 105, 64, 229, 91, 63, 67, 85]

f = open('hinh.jpg', 'w+b')
binary_f = bytearray(byte_arr)
f.write(binary_f)
f.close()