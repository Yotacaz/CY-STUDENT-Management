Pour lancer le programme : depuis ce repertoire utiliser : 
make all
puis lancer l'executable. En cas de problème lors de la compilation, penser à utiliser 
make clean

##Fonctionnalités :
- chargement des données de data.txt en mémoire
- obtenir le top x des étudiants avec get_top_students, 
- afficher différentes structure (comme avec print_student, print_promotion --> pas affinée)
- liberation des donnees

La liste de tous les cours possible est gérée avec course.c, une promotion contient l'ensemble des étudiants ainsi que la liste des cours possible. Chaque étudiant est assigné à toutes les matières (qui sont gérées dans followed_course.c). Les macro permettant de définir des tableaux dynamiques sont situées dans dyn_table.h
---
N'ayant pas pu finir le travail, je le poursuivrais pendant les vacances. 
Je n'ai malheureusement pas fait la documentation au fur et à mesure.

J'ai utilisé des macros pour définir des structures de tableaux dynamiques pouvant contenir le type de donnée souhaité. Sa mise en place et son debug m'ont pris plus de temps que prévu. J'ai sans doute aussi passé trop de temps sur des variantes de structures des fichiers et sur plusieurs versions du makefile (sans grand succès).


