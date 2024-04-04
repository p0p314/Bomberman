
class Debouncer
{ // Class servant à bloquer les envois de paquets par le serveur.
    // Si le serveur envoie trop de paquets au joueur celui-ci n'arrive pas à afficher dans le bonne ordre les différentes actions.
public:
    Debouncer(float delay);
    bool update(float deltaTime);
    void trigger();
    bool isReady();

private:
    float delay;
    float elapsedTime;
    bool ready;
};
