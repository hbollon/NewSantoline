//une correction ligne 24, on rajoute LISTAUX


function [LISTFF,LISTAUX]=uneiteration(LISTFF,LISTAUX,IND)


    MVT = LISTFF(IND);
    MT=MVT(1);
    VITS=MVT(2);
    MM=MT([1,2],:);
    tauperp=MT([4,5],:);
    if norm(MM-round(MM))<= eps then
        disp(MVT,'on est sur un sommet');
        //[LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
        //pause;
    end

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CALCUL DES VITESSES DE PROPAGATION
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    disp(['MT(9)= ',string(MT(9))]);
    if MT(9)==0 then
        [VITS,INDIC]=vitesse_new_2(VITS,tauperp)
    elseif MT(9)==1
        [VITS,INDIC]=vitesse_choc_new(VITS,tauperp);
    elseif MT(9)==2
        [VITS,INDIC,LISTAUX]=vitesse_sommet_new(VITS,tauperp,LISTAUX);//********* correction **************
    elseif MT(9)==3 // cas d'un père de choc artificiel cf 'chocdansunecellule'
        VITS=list(LISTFF(IND)(2));
        INDIC=%T;
    else
        disp("erreur dans MT(9)");
        abort;
    end

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SUPPRESSION DU POINT SI PAS DE VITESSE CALCULEE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if INDIC==%F then
        disp(['PAS DE VITESSE CALCULEE, on supprime IND',string(IND)]);
        [LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
    else
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PROPAGATION DU POINT
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        [LISTFF,LISTAUX,INDIC]=propage(LISTFF,LISTAUX,IND,VITS);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SUPPRESSION DU POINT SI PAS DE PROPAGATION POSSIBLE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if INDIC == %F then
        disp(['PAS DE PROPAGATION POSSIBLE, on supprime IND',string(IND)]);
        [LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
        end
    end

    endfunction
