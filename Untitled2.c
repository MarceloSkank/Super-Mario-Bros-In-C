
#include <allegro5/allegro.h>
#include<math.h>
#include <allegro5/allegro_image.h>

#define FPS 60.0
#define Largura 640
#define Altura 480

struct inimigo{
    int x;
    int y;
    int Al,La;
    int SpawnPoint;
    int Spawnado;

};
int CountInimigo = 0;

struct inimigo SetInimigo[20];
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_EVENT_QUEUE *FilaEvento;
ALLEGRO_TIMER *tempo = NULL;
ALLEGRO_BITMAP *InimigoImagem;
ALLEGRO_BITMAP *Chao;
ALLEGRO_BITMAP *Buraco;

int x;
int y;
int xbg4;
void SpawnInimigo(){

        SetInimigo[CountInimigo].y = al_get_display_height(janela)-30;
        SetInimigo[CountInimigo].La = al_get_bitmap_width(InimigoImagem);
        SetInimigo[CountInimigo].Al = al_get_bitmap_height(InimigoImagem);
        SetInimigo[CountInimigo].x = al_get_display_width(janela) - SetInimigo[CountInimigo].La;
        SetInimigo[CountInimigo].Spawnado = 1;
        CountInimigo++;
        if(CountInimigo >= 20){
            CountInimigo = 0;
        }
}

int Colisao(int Indice){
    int BDMario = x + al_get_bitmap_width(imagem);

    int BDInimigo = SetInimigo[Indice].x + SetInimigo[Indice].La;
    int BEInimigo = SetInimigo[Indice].x;
    int  BEMario = x;

    int BBMario = y + al_get_bitmap_height(imagem);
    int BBInimigo = SetInimigo[Indice].y + SetInimigo[Indice].Al;
    int BCInimigo = SetInimigo[Indice].y;
    int  BCMario = y;
    if(BDMario >= BEInimigo && BEMario < BDInimigo && BBMario >= BCInimigo && BCMario < BBInimigo){
            if(BDMario >= BEInimigo){
                return 1;
            }
            if(BEMario <= BDInimigo){
                return 2;
            }
            if(BBMario >= BCInimigo){
                return 3;
            }
            if(BCMario <= BBInimigo){
                return 4;
            }

            //return 1;

    }else{
        return 0;
    }

}
int ColisaoBuraco(){
    int BDMario = x + al_get_bitmap_width(imagem);
    int BDBuraco = xbg4 + 100;
    int BEBuraco = xbg4;
    int  BEMario = x;
    if(BDMario >= BEBuraco && BEMario < BDBuraco){
            if(BDMario >= BEBuraco){
                return 1;
            }
            if(BEMario <= BDBuraco){
                return 2;
            }




    }else{
        return 0;
    }

}
int main(void){
   al_init();

    al_init_image_addon();
    al_install_keyboard();

    SetInimigo[0].SpawnPoint = -1000;
    SetInimigo[0].Spawnado = 0;
    SetInimigo[1].SpawnPoint = -2000;
    SetInimigo[1].Spawnado = 0;
    SetInimigo[2].SpawnPoint = -3000;
    SetInimigo[2].Spawnado = 0;

    janela = al_create_display(640, 480);
    xbg4 = al_get_display_width(janela);

    InimigoImagem = al_load_bitmap("goombas.bmp");
    imagem = al_load_bitmap("mario.bmp");
    Chao = al_load_bitmap("Chao.png");

    Buraco = al_create_bitmap(100,100);
    al_set_target_bitmap(Buraco);
    al_clear_to_color(al_map_rgb(107,140,255));
    al_set_target_bitmap(al_get_backbuffer(janela));

    al_convert_mask_to_alpha(imagem,al_map_rgb(255,0,255));
    al_convert_mask_to_alpha(InimigoImagem,al_map_rgb(255,0,255));
    al_set_window_title(janela,"Super Mario Bros");

    tempo = al_create_timer(1/FPS);
    FilaEvento = al_create_event_queue();
    al_register_event_source(FilaEvento,al_get_display_event_source(janela));
    al_register_event_source(FilaEvento,al_get_timer_event_source(tempo));
    al_register_event_source(FilaEvento,al_get_keyboard_event_source());

    al_start_timer(tempo);
    int sair = 0;
    int desenha = 1;


    x = 0;
    y = al_get_display_height(janela)-30;
    float yf = 0;
    int tecla = 0;
    int vira = 0;
    int count = 0;
    float ct = 0;
    int SpawnBuraco = -1000;
    float Yboneco = 0;
    int pulando = 0;
    int ap = 200;
    int xbg = 0,Xbg2 = xbg+al_get_bitmap_width(Chao),xbg3 = al_get_bitmap_width(Chao)*2,xbg1=0;
    while(!sair){
         ALLEGRO_EVENT evento;
         al_wait_for_event(FilaEvento,&evento);
        if(evento.type == ALLEGRO_EVENT_TIMER){
             if(tecla == 1){
                vira = 0;
                if(x >= al_get_display_width(janela)* 0.7){
                    xbg-=5;
                    xbg1-=5;
                    Xbg2 -=5;
                    xbg3 -=5;
                    if(xbg < SpawnBuraco + 100){
                        xbg4 -=5;
                    }
                    int i;
                    for(i = 0; i < 3; i++){
                        if(xbg <= SetInimigo[i].SpawnPoint && SetInimigo[i].Spawnado == 0){
                            SpawnInimigo();
                        }

                        if(SetInimigo[i].Spawnado == 1){
                            SetInimigo[i].x -= 5;
                        }

                    }

                }else{
                    x+= 5 * tecla;
                }
             }
            if(tecla == -1){
                x+= 5 * tecla;
                vira  = 1;

            }
            if(x <= 0){
                x = 0;
            }
            if(pulando == 1){
                ct += 1/FPS;
                yf = 640 * pow(ct,4) - 1280 * pow(ct,3) + 1400*pow(ct,2) - 760 * ct;
                y = Yboneco+yf;
                if(ct > 1 && !ColisaoBuraco()){
                    pulando = 0;
                    y = Yboneco;
                }
            }
            int i;
            for(i = 0; i < 3; i++){
                if(SetInimigo[i].Spawnado == 1){
                    SetInimigo[i].x -=1;
                    if(Colisao(i)){
                        sair = 1;

                    }
                }

            }
            if(ColisaoBuraco()){
                y+= 5;
            }
            if(y > al_get_display_height(janela)+100){
                sair = 1;

            }
            desenha = 1;


        }
        if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            sair = 1;

        }

        if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(evento.keyboard.keycode){
            case ALLEGRO_KEY_LEFT:
                count++;
                tecla = -1;
                break;
            case ALLEGRO_KEY_RIGHT:
                tecla = 1;
                count++;
                break;
            case ALLEGRO_KEY_ESCAPE:
                sair = 1;
                break;
            case ALLEGRO_KEY_SPACE:
                if (pulando == 0){
                    pulando = 1;
                    ct = 0;
                    Yboneco = y;
                }
                break;
            }
        }

        if(evento.type == ALLEGRO_EVENT_KEY_UP){
            if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT || evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
            count --;
            if(count == 0){
                tecla = 0;
            }
            else if(evento.keyboard.keycode == ALLEGRO_KEY_LEFT){
                tecla = 1;
            }
            else if(evento.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                tecla = -1;
            }
        }
        }
        if(desenha){
            al_clear_to_color(al_map_rgb(107,140,255));
            al_draw_bitmap(Chao,xbg1,al_get_display_height(janela)-al_get_bitmap_height(Chao),0);
            al_draw_bitmap(Chao,Xbg2,al_get_display_height(janela)-al_get_bitmap_height(Chao),0);
            al_draw_bitmap(Chao,xbg3,al_get_display_height(janela)-al_get_bitmap_height(Chao),0);


            if(xbg1 < -al_get_bitmap_width(Chao)){
                xbg1 = 0;
                Xbg2 = al_get_bitmap_width(Chao);
                xbg3 = al_get_bitmap_width(Chao)*2;


            }

            if(xbg < SpawnBuraco + 100){
                al_draw_bitmap(Buraco,xbg4,al_get_display_height(janela)-100,0);
            }
            al_draw_bitmap(imagem,x,y-60,vira);
            int i;
            for(i = 0; i < 3; i++){
                if(SetInimigo[i].Spawnado == 1)
                    al_draw_bitmap(InimigoImagem,SetInimigo[i].x,SetInimigo[i].y-60,0);
            }

            al_flip_display();
            desenha  = 0;
        }
    }

    al_destroy_display(janela);

    return 0;
}
