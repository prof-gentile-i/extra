document.addEventListener("DOMContentLoaded", () => {
  const assegnaBtn = document.getElementById("assegna-btn");
  const banchi = document.querySelectorAll("td");

  assegnaBtn.addEventListener("click", () => {
    // Step 1: Resettare lo stato dei banchi
    banchi.forEach((banco, index) => {
      banco.classList.remove("banco-assegnato");
      // Ripristina solo il numero del banco
      banco.innerHTML = `<div class="banco-content"><span class="numero-banco">${
        index + 1
      }</span></div>`;
    });

    // Step 2: Caricare il file degli alunni
    fetch("students.txt")
      .then((response) => {
        if (!response.ok) {
          throw new Error(
            "Errore durante il caricamento del file students.txt. Verifica che esista nella stessa cartella."
          );
        }
        return response.text();
      })
      .then((data) => {
        const nomiAlunni = data
          .split("\n")
          .filter((nome) => nome.trim() !== ""); // Rimuove righe vuote

        // Step 3: Mescolare i nomi degli alunni (algoritmo di Fisher-Yates)
        for (let i = nomiAlunni.length - 1; i > 0; i--) {
          const j = Math.floor(Math.random() * (i + 1)); // Indice casuale da 0 a i
          [nomiAlunni[i], nomiAlunni[j]] = [nomiAlunni[j], nomiAlunni[i]];
        }

        // Step 4: Convertire la lista di banchi in un array e mescolarla (algoritmo di Fisher-Yates)
        const banchiArray = Array.from(banchi);
        for (let i = banchiArray.length - 1; i > 0; i--) {
          const j = Math.floor(Math.random() * (i + 1));
          [banchiArray[i], banchiArray[j]] = [banchiArray[j], banchiArray[i]];
        }

        // Step 5: Assegnare gli alunni ai banchi casuali
        nomiAlunni.forEach((nomeAlunno, index) => {
          // Controlla se ci sono abbastanza banchi per tutti gli studenti
          if (index < banchiArray.length) {
            const bancoCasuale = banchiArray[index];
            const numeroBanco = parseInt(bancoCasuale.textContent.trim());

            // Aggiorna il contenuto del banco con il nome
            bancoCasuale.innerHTML = `<div class="banco-content"><span class="numero-banco">${numeroBanco}</span><div class="nome-alunno">${nomeAlunno}</div></div>`;
            bancoCasuale.classList.add("banco-assegnato");
          }
        });
      })
      .catch((error) => {
        console.error("Si è verificato un errore:", error);
        alert("Errore: " + error.message);
      });
  });
});
