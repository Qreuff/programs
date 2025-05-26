import pygame
import sys
import random
from pygame.locals import *

#константы
FPS = 60
LANE_COUNT = 4
NOTE_SPEED = 5
BASE_SCORE = 100
COMBO_STEP = 5
COMBO_MULTIPLIER_INCREMENT = 0.2

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GRAY = (50, 50, 50)
PAUSE_OVERLAY_COLOR = (0, 0, 0, 150)  #сделал полупрозрачный фон во время паузы

class Note(pygame.sprite.Sprite):
    def __init__(self, lane, y=0, index=0):
        super().__init__()
        self.lane = lane
        self.index = index

        self.image = pygame.Surface((50, 50), pygame.SRCALPHA)
        color = self.random_color()
        pygame.draw.circle(self.image, color, (25, 25), 20)

        #нарисовал цифру каждому кружочку
        font = pygame.font.Font(None, 24)
        text = font.render(str(self.index), True, (0, 0, 0))
        text_rect = text.get_rect(center=(25, 25))
        self.image.blit(text, text_rect)

        self.rect = self.image.get_rect()
        self.rect.x = 200 + lane * 100
        self.rect.y = y

    def random_color(self): #добавил цвет кружочкам 
        while True:
            color = tuple(random.randint(0, 255) for _ in range(3))
            if color != (0, 0, 0):  #исключаю черный цвет, чтобы не было такого что кружок не видно 
                return color

    def update(self):
        self.rect.y += NOTE_SPEED

class Game:
    def __init__(self):
        pygame.mixer.pre_init(44100, -16, 2, 512)
        pygame.init()
        try:
            pygame.mixer.init()
        except pygame.error:
            print("Звук не доступен, продолжаем без звука.")
        self.info = pygame.display.Info()
        self.screen_width = self.info.current_w
        self.screen_height = self.info.current_h
        self.screen = pygame.display.set_mode((800, 600))
        pygame.display.set_caption("Piano")
        self.clock = pygame.time.Clock()
        self.running = True
        self.state = "menu"
        self.sound_on = True
        self.fullscreen = False

        #эффекты для меню
        self.bg_offset = 0
        self.title_alpha = 255
        self.title_fade_dir = -1
        
        self.title_angle = 0
        self.title_angle_dir = 1
        self.max_angle = 5
        self.angle_speed = 0.1
        
        self.particles = []
        for _ in range(50):
            self.particles.append({
                "x": random.randint(0, 800),
                "y": random.randint(0, 600),
                "speed": random.uniform(0.3, 1.5),
                "size": random.randint(1, 4),
                "color": (random.randint(100, 200), 
                          random.randint(100, 200), 
                          random.randint(200, 255))
            })

        #эффекты для настроек
        self.settings_particles = []
        for _ in range(30):
            self.settings_particles.append({
                "x": random.randint(0, 800),
                "y": random.randint(0, 600),
                "speed": random.uniform(0.2, 1.0),
                "size": random.randint(1, 3),
                "color": (random.randint(50, 100), 
                          random.randint(50, 100), 
                          random.randint(100, 150))
            })

        self.score = 0
        self.combo = 0
        self.multiplier = 1.0

        self.notes = pygame.sprite.Group()

        self.note_chart = []
        self.note_index = 0
        self.level_start_time = 0

        self.missed_notes = 0
        self.hit_notes = 0

        self.paused = False
        
        #эффекты для клавиш в игре
        self.key_effects = {
            0: {"active": False, "timer": 0, "color": (255, 0, 0)},
            1: {"active": False, "timer": 0, "color": (0, 255, 0)},
            2: {"active": False, "timer": 0, "color": (0, 0, 255)},
            3: {"active": False, "timer": 0, "color": (255, 255, 0)}
        }

    def run(self):
        while self.running:
            if self.state == "menu":
                self.menu_loop()
            elif self.state == "settings":
                self.settings_loop()
            elif self.state == "level_select":
                self.level_select_loop()
            elif self.state == "play":
                if not self.paused:
                    self.play_loop()
                else:
                    self.pause_loop()
            elif self.state == "game_over":
                self.game_over_loop()
            self.clock.tick(FPS)
        pygame.quit()
        sys.exit()

    def menu_loop(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                self.running = False
            elif event.type == KEYDOWN:
                if event.key == K_s:
                    self.state = "level_select"
                elif event.key == K_o:
                    self.state = "settings"
                elif event.key == K_q:
                    self.running = False
            elif event.type == MOUSEBUTTONDOWN:
                mouse_pos = pygame.mouse.get_pos()
                if 300 <= mouse_pos[0] <= 500:
                    if 230 <= mouse_pos[1] <= 270:
                        self.state = "level_select"
                    elif 280 <= mouse_pos[1] <= 320:
                        self.state = "settings"
                    elif 330 <= mouse_pos[1] <= 370:
                        self.running = False

        # Анимированный фон
        self.bg_offset = (self.bg_offset + 0.5) % 600
        self.screen.fill(BLACK)

        # Частицы
        for p in self.particles:
            p["y"] += p["speed"]
            if p["y"] > 600:
                p["y"] = 0
                p["x"] = random.randint(0, 800)
            pygame.draw.circle(
                self.screen,
                p["color"],
                (int(p["x"]), int(p["y"])),
                p["size"]
            )

        # Мерцающий заголовок
        self.title_alpha += self.title_fade_dir * 2
        if self.title_alpha <= 100 or self.title_alpha >= 255:
            self.title_fade_dir *= -1

        self.title_angle += self.title_angle_dir * self.angle_speed
        
        if self.title_angle >= self.max_angle or self.title_angle <= -self.max_angle:
            self.title_angle_dir *= -1
            
        font = pygame.font.Font(None, 64)
        small_font = pygame.font.Font(None, 36)
        hover_font = pygame.font.Font(None, 40)
        
        title_orig = font.render("Piano", True, WHITE)
        title_orig.set_alpha(self.title_alpha)
        
        title_rotated = pygame.transform.rotate(title_orig, self.title_angle)
        
        title_rect = title_rotated.get_rect(center=(800 // 2, 100 + title_rotated.get_height() // 2))
        
        self.screen.blit(title_rotated, title_rect)

        # Интерактивные кнопки
        mouse_pos = pygame.mouse.get_pos()
        buttons = [
            {"text": "Начать игру [S]", "y": 250, "hover": False, "action": "level_select"},
            {"text": "Настройки [O]", "y": 300, "hover": False, "action": "settings"},
            {"text": "Выйти [Q]", "y": 350, "hover": False, "action": "quit"},
        ]

        for btn in buttons:
            btn_rect = pygame.Rect(300, btn["y"] - 20, 200, 40)
            btn["hover"] = btn_rect.collidepoint(mouse_pos)
            
            current_font = hover_font if btn["hover"] else small_font
            color = (77, 78, 163) if btn["hover"] else WHITE
            
            text_surf = current_font.render(btn["text"], True, color)
            self.screen.blit(text_surf, (400 - text_surf.get_width() // 2, btn["y"] - 20))
            
        pygame.display.flip()

    def settings_loop(self):
        
        font = pygame.font.Font(None, 36)
        title_font = pygame.font.Font(None, 48)
        hover_font = pygame.font.Font(None, 40)
        
        self.title_alpha += self.title_fade_dir * 2
        if self.title_alpha <= 100 or self.title_alpha >= 255:
            self.title_fade_dir *= -1
        
        self.title_alpha += self.title_angle_dir * self.angle_speed
        if self.title_angle >= self.max_angle or self.title_angle <= -self.max_angle:
            self.title_angle_dir *= -1
            
        for event in pygame.event.get():
            self.title_angle += self.title_angle_dir * self.angle_speed
            if self.title_angle >= self.max_angle or self.title_angle <= -self.max_angle:
                self.title_angle_dir *= -1
            if event.type == QUIT:
                self.running = False
            elif event.type == KEYDOWN:
                if event.key == K_f:
                    self.fullscreen = not self.fullscreen
                    self.toggle_fullscreen()
                elif event.key == K_m:
                    self.sound_on = not self.sound_on
                elif event.key == K_b:
                    self.state = "menu"
            elif event.type == MOUSEBUTTONDOWN:
                mouse_pos = event.pos

                settings_click_areas = [
                    {"y": 200, "action": "toggle_fullscreen"},
                    {"y": 250, "action": "toggle_sound"},
                    {"y": 300, "action": "back_to_menu"}
                ]
                for btn_data in settings_click_areas:#кнопки в настройках
                    if btn_data["action"] == "toggle_fullscreen":
                        current_text = f'Полноэкранный режим: {'ВКЛ' if self.fullscreen else 'ВЫКЛ'}[F]'
                    elif btn_data["action"] == "toggle_sound":
                        current_text = f'Звук {'ВКЛ' if self.sound_on else 'ВЫКЛ'}[M]'
                    else:
                        current_text = "Назад [B]"
                    
                    
                    text_width = font.render(current_text, True, WHITE).get_width()
                    btn_rect = pygame.Rect(100, btn_data["y"] - 18, text_width + 20, 36)
                    
                    if btn_rect.collidepoint(mouse_pos):
                        if btn_data["action"] == "toggle_fullscreen":
                            self.fullscreen = not self.fullscreen
                            self.toggle_fullscreen()
                        elif btn_data["action"] == "toggle_sound":
                            self.sound_on = not self.sound_on
                        elif btn_data["action"] == "back_to_menu":
                            self.state = "menu"
        #анимировал фон для настроек
        self.screen.fill(GRAY)
        
        #сделал частицы для настроек
        for p in self.settings_particles:
            p["y"] += p["speed"]
            if p["y"] > 600:
                p["y"] = 0
                p["x"] = random.randint(0, 800)
            pygame.draw.circle(
                self.screen,
                p["color"],
                (int(p["x"]), int(p["y"])),
                p["size"]
            )
        
        # мерцающий заголовок
        title_orig = title_font.render("Настройки", True, WHITE)
        title_orig.set_alpha(self.title_alpha)
        title_rotated = pygame.transform.rotate(title_orig, self.title_angle)
        title_rect = title_rotated.get_rect(center=(800 // 2, 100 + title_rotated.get_height() // 2))
        self.screen.blit(title_rotated, title_rect)
        
        mouse_pos = pygame.mouse.get_pos()
        settings_display_buttons = [
            {"text_base": "Полноэкранный режим:", "key_action": "F", "action": "toggle_fullscreeen", "y": 200},
            {"text_base": "Звук:", "key_action": "M", "action": "toggle_sound", "y": 250},
            {"text_base": "Назад", "key_action": "B", "action": "back_to_menu", "y": 300}
        ]
        
        for btn_data in settings_display_buttons:
            if btn_data["action"] == "toggle_fullscreen":
                current_text = f"{btn_data['text_base']}{'ВКЛ' if self.fullscreen else 'Выкл'}[{btn_data['key_action']}]"
            elif btn_data["action"] == "toggle_sound":
                current_text = f"{btn_data['text_base']}{'ВКЛ' if self.sound_on else 'Выкл'}[{btn_data['key_action']}]"
            else:
                current_text = f"{btn_data['text_base']}[{btn_data['key_action']}]"
                
            text_width = font.render(current_text, True, WHITE).get_width()
            btn_rect = pygame.Rect(100, btn_data["y"] - 18, text_width + 20, 36)
            
            is_hovered = btn_rect.collidepoint(mouse_pos)
            current_font_to_render = hover_font if is_hovered else font
            color_to_render = (77, 78, 163) if is_hovered else WHITE
            
            text_surf = current_font_to_render.render(current_text, True, color_to_render)
            self.screen.blit(text_surf, (100, btn_data["y"]))
        pygame.display.flip()

    def toggle_fullscreen(self):
        if self.fullscreen:
            self.screen = pygame.display.set_mode((self.info.current_w, self.info.current_h), FULLSCREEN)
        else:
            self.screen = pygame.display.set_mode((800, 600))

    def level_select_loop(self):
        # Объявляем шрифты в начале функции, чтобы они были доступны везде
        font = pygame.font.Font(None, 36)
        hover_font = pygame.font.Font(None, 40) 

        # Определяем кнопки и их действия/координаты
        level_buttons_data = [
            {"text": "Urotsuki - Lavender Waters [1]", "y": 200, "action": 1},
            {"text": "soon..", "y": 250, "action": 2}, 
            {"text": "Назад [B]", "y": 300, "action": "back_to_menu"}
        ]

        for event in pygame.event.get():
            if event.type == QUIT:
                self.running = False
            elif event.type == KEYDOWN:
                if event.key == K_1:
                    self.start_level(1)
                elif event.key == K_2:
                    pass 
                elif event.key == K_b:
                    self.state = "menu"
            elif event.type == MOUSEBUTTONDOWN:
                mouse_pos = event.pos

                for btn_data in level_buttons_data:
                    temp_text_surf = font.render(btn_data["text"], True, WHITE)
                    text_width = temp_text_surf.get_width()
                    
                    btn_rect = pygame.Rect(self.screen.get_width() // 2 - text_width // 2 - 10, 
                                           btn_data["y"] - 18, 
                                           text_width + 20, 
                                           36)

                    if btn_rect.collidepoint(mouse_pos):
                        if isinstance(btn_data["action"], int): 
                            if btn_data["action"] == 2: 
                                pass 
                            else:
                                self.start_level(btn_data["action"])
                        elif btn_data["action"] == "back_to_menu":
                            self.state = "menu"

        self.screen.fill(BLACK)

        mouse_pos = pygame.mouse.get_pos()
        for btn_data in level_buttons_data:
            temp_text_surf = font.render(btn_data["text"], True, WHITE)
            text_width = temp_text_surf.get_width()
            
            btn_rect = pygame.Rect(self.screen.get_width() // 2 - text_width // 2 - 10, 
                                   btn_data["y"] - 18, 
                                   text_width + 20, 
                                   36)
            
            is_hovered = btn_rect.collidepoint(mouse_pos)
            current_font = hover_font if is_hovered else font
            color_to_render = (77, 78, 163) if is_hovered else WHITE
    
            text_surf = current_font.render(btn_data["text"], True, color_to_render)
            
            # Выравниваем текст по центру
            self.screen.blit(text_surf, (self.screen.get_width() // 2 - text_surf.get_width() // 2, btn_data["y"]))
            
        pygame.display.flip()

    def start_level(self, level):
        self.state = "play"
        self.notes.empty()
        self.score = 0
        self.combo = 0
        self.multiplier = 1.0
        self.missed_notes = 0
        self.hit_notes = 0
        self.paused = False

        if self.sound_on:
            try:
                pygame.mixer.music.load(r"/Users/mac/Documents/1/rgr(proga)/Urotsuki - Lavender Waters.mp3")
                pygame.mixer.music.play()
            except Exception as e:
                print("Ошибка загрузки музыки:", e)

        self.level_start_time = pygame.time.get_ticks()
        self.note_chart = [
            (370, 0),  # 1
            (1380, 1),  # 2
            (1950, 2),  # 3
            (2500, 3),  # 4
            (2900, 0),  # 5
            (3500, 1),  # 6
            (4000, 2),  # 7
            (4200, 0),  # 8
            (4540, 1),  # 9
            (4800, 3),  # 10
            (5000, 2),
            (5500, 2),
            (6500, 3),
            (6650, 0),
            (7300, 2),
            (7800, 3),
            (8300, 0),
            (8800, 3),
            (9900, 2),
            (10500, 1),  # 20
            (10950, 0),
            (12500, 3),
            (13150, 2),
            (13600, 0),
            (14200, 1),
            (14700, 1),
            (15200, 3),
            (15700, 0),
            (16300, 2),
            (17000, 0),  # 30
            (17200, 1),
            (17500, 3),
            (18450, 0),
            (19000, 2),
            (19600, 1),
            (21200, 3),
            (21400, 2),
            (21700, 3),
            (22000, 1),
            (22222, 2),  # 40
            (22500, 1),
            (22700, 0),
            (23000, 1),
            (23300, 0),
            (23900, 3),
            (25000, 0),
            (25500, 2),
            (26050, 2),
            (26500, 0),
            (27100, 3),  # 50
            (27550, 0),
            (28200, 2),
            (29900, 3),
            (30300, 0),
            (30800, 3),
        ]
        self.note_index = 0

    def play_loop(self):
        current_time = pygame.time.get_ticks() - self.level_start_time

        for event in pygame.event.get():
            if event.type == QUIT:
                self.running = False
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    self.paused = True
                    if pygame.mixer.get_init():
                        pygame.mixer.music.pause()
                else:
                    self.handle_key(event.key)

        # Обновляем эффекты клавиш
        for lane in self.key_effects:
            if self.key_effects[lane]["active"]:
                self.key_effects[lane]["timer"] += 1
                if self.key_effects[lane]["timer"] > 10:  # Длительность эффекта
                    self.key_effects[lane]["active"] = False
                    self.key_effects[lane]["timer"] = 0

        # Создаём новые ноты по времени
        while self.note_index < len(self.note_chart) and self.note_chart[self.note_index][0] <= current_time:
            lane = self.note_chart[self.note_index][1]
            self.notes.add(Note(lane, index=self.note_index + 1))
            self.note_index += 1

        self.notes.update()
        self.check_missed_notes()

        # Если ноты закончились и все убраны - игра окончена
        if self.note_index == len(self.note_chart) and len(self.notes) == 0:
            self.state = "game_over"
            pygame.mixer.music.stop()

        self.screen.fill(BLACK)

        for i in range(LANE_COUNT):
            pygame.draw.line(self.screen, GRAY, (200 + i * 100, 0), (200 + i * 100, self.screen.get_height()))

        hit_line_y = self.screen.get_height() - 100
        pygame.draw.line(self.screen, WHITE, (150, hit_line_y), (650, hit_line_y), 5)

        self.notes.draw(self.screen)
        self.draw_score()
        self.draw_controls(hit_line_y)
        pygame.display.flip()

    def handle_key(self, key):
        lane_keys = {K_q: 0, K_w: 1, K_e: 2, K_r: 3}
        if key in lane_keys:
            lane = lane_keys[key]
            # Активируем эффект для этой клавиши
            self.key_effects[lane]["active"] = True
            self.key_effects[lane]["timer"] = 0
            
            for note in self.notes:
                if note.lane == lane and abs(note.rect.y - (self.screen.get_height() - 100)) < 30:
                    self.notes.remove(note)
                    self.register_hit()
                    break

    def register_hit(self):
        self.combo += 1
        self.hit_notes += 1
        if self.combo % COMBO_STEP == 0:
            self.multiplier += COMBO_MULTIPLIER_INCREMENT
        self.score += int(BASE_SCORE * self.multiplier)

    def check_missed_notes(self):
        for note in list(self.notes):
            if note.rect.y > self.screen.get_height():
                self.notes.remove(note)
                self.combo = 0
                self.multiplier = 1.0
                self.missed_notes += 1
                # Если пропущено 10 и больше - конец игры
                if self.missed_notes >= 10:
                    self.state = "game_over"
                    pygame.mixer.music.stop()

    def draw_score(self):
        font = pygame.font.Font(None, 36)
        score_surf = font.render(f"Score: {self.score}", True, WHITE)
        combo_surf = font.render(f"Combo: {self.combo}", True, WHITE)
        self.screen.blit(score_surf, (10, 10))
        self.screen.blit(combo_surf, (10, 50))

    def draw_controls(self, hit_line_y):
        font = pygame.font.Font(None, 28)
        keys = ['Q', 'W', 'E', 'R']
        start_x = 200
        text_y = hit_line_y + 15
        effect_y_offset = 9
        
        for i, key in enumerate(keys):
            # Рисуем эффект нажатия, если он активен
            if self.key_effects[i]["active"]:
                effect_size = 50 - self.key_effects[i]["timer"] * 3
                if effect_size > 0:
                    effect_surf = pygame.Surface((effect_size, effect_size), pygame.SRCALPHA)
                    pygame.draw.circle(
                        effect_surf, 
                        (*self.key_effects[i]["color"], 150 - self.key_effects[i]["timer"] * 15), 
                        (effect_size // 2, effect_size // 2), 
                        effect_size // 2
                    )
                    self.screen.blit(
                        effect_surf, 
                        (start_x + i * 100 + 25 - effect_size // 2, 
                         text_y - 25 + (50 - effect_size) // 2 + effect_y_offset)
                    )
            
            # Рисуем текст клавиши
            text = font.render(key, True, WHITE)
            x = start_x + i * 100 + 25 - text.get_width() // 2
            self.screen.blit(text, (x, text_y))

    def pause_loop(self):
        font = pygame.font.Font(None, 72)
        font_small = pygame.font.Font(None, 36)
        hover_font = pygame.font.Font(None, 40)
        
        for event in pygame.event.get():
            if event.type == QUIT:
                self.running = False
            elif event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    self.paused = False
                    if pygame.mixer.get_init():
                        pygame.mixer.music.unpause()
                elif event.key == K_m:
                    self.state = "menu"
                    if pygame.mixer.get_init():
                        pygame.mixer.music.stop()
            elif event.type == MOUSEBUTTONDOWN:
                mouse_pos = event.pos

                pause_click_areas = [
                    {"text": "Продолжить [Esc]", "y": 300, "action": "resume"},
                    {"text": "В меню [M]", "y": 350, "action": "back_to_menu"}
                ]
                
                for btn_data in pause_click_areas:
                    text_width = font_small.render(btn_data["text"], True, WHITE).get_width()
                    btn_rect = pygame.Rect(self.screen.get_width() // 2 - text_width // 2 - 10, btn_data["y"] - 18, text_width + 20, 36)
                    
                    if btn_rect.collidepoint(mouse_pos):
                        if btn_data["action"] == "resume":
                            self.paused = False
                            pygame.mixer.music.unpause()
                        elif btn_data["action"] == "back_to_menu":
                            self.state = "menu"
                            pygame.mixer.music.stop()
        # Полупрозрачный затухающий экран
        overlay = pygame.Surface(self.screen.get_size(), pygame.SRCALPHA)
        overlay.fill(PAUSE_OVERLAY_COLOR)
        self.screen.blit(overlay, (0, 0))

        pause_text = font.render("ПАУЗА", True, WHITE)
        self.screen.blit(pause_text, (self.screen.get_width() // 2 - pause_text.get_width() // 2, 200))

        mouse_pos = pygame.mouse.get_pos()
        pause_display_buttons = [
            {"text": "Продолжить [Esc]", "y": 300, "action": "resume"},
            {"text": "В меню [M]", "y": 350, "action": "back_to_menu"}
        ]
        for btn_data in pause_display_buttons:
            text_width = font_small.render(btn_data["text"], True, WHITE).get_width()
  
            btn_rect = pygame.Rect(self.screen.get_width() // 2 - text_width // 2 - 10, btn_data["y"] - 18, text_width + 20, 36) 
            
            is_hovered = btn_rect.collidepoint(mouse_pos)
            current_font_to_render = hover_font if is_hovered else font_small
            color_to_render = (77, 78, 163) if is_hovered else WHITE
            
            text_surf = current_font_to_render.render(btn_data["text"], True, color_to_render)
            self.screen.blit(text_surf, (self.screen.get_width() // 2 - text_surf.get_width() // 2, btn_data["y"]))
            
            
        pygame.display.flip()
            
    def game_over_loop(self):
        for event in pygame.event.get():
            if event.type == QUIT:
                self.running = False
            elif event.type == KEYDOWN:
                if event.key == K_b:
                    self.state = "menu"

        self.screen.fill(BLACK)
        font_big = pygame.font.Font(None, 72)
        font_small = pygame.font.Font(None, 36)

        over_text = font_big.render("GAME OVER", True, WHITE)
        self.screen.blit(over_text, (self.screen.get_width() // 2 - over_text.get_width() // 2, 100))

        stats = [
            f"Очков: {self.score}",
            f"Попаданий: {self.hit_notes}",
            f"Промахов: {self.missed_notes}",
            "В меню [B]"
        ]

        for i, line in enumerate(stats):
            text_surf = font_small.render(line, True, WHITE)
            self.screen.blit(text_surf, (self.screen.get_width() // 2 - text_surf.get_width() // 2, 250 + i * 40))

        pygame.display.flip()

if __name__ == "__main__":
    Game().run()