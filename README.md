# 🎯 FocusHub

Aplicație de productivitate cu interfață în **HTML/CSS/JavaScript**, ce rulează atât ca aplicație desktop (**Electron**), cât și direct în browser (ex. GitHub Pages). Permite gestionarea sarcinilor zilnice, a unei liste "someday" (wishlist) și a unui spațiu creativ pentru idei ("Dreams" — mind map, scratchpad, citate), cu persistență locală a datelor.

## Cuprins

- [Funcționalități](#funcționalități)
- [Arhitectură](#arhitectură)
- [Structura proiectului](#structura-proiectului)
- [Cerințe](#cerințe)
- [Instalare și rulare](#instalare-și-rulare)
- [Build (aplicație desktop)](#build-aplicație-desktop)
- [Demo live (browser)](#demo-live-browser)
- [Detalii de implementare](#detalii-de-implementare)
- [Limitări cunoscute / TODO](#limitări-cunoscute--todo)

## Funcționalități

**Tab Focus**
- Adăugare, editare, ștergere task-uri (titlu, descriere, subtask-uri, categorie, nivel de energie, deadline, durată estimată).
- Grupare automată pe categorii, cu secțiuni pliabile.
- Reordonare task-uri prin drag & drop, în cadrul aceleiași categorii.
- Bară de progres zilnică și pastilă rezumat ("X/Y complete • timp rămas").
- Sortare inteligentă (deadline + nivel de energie, cu boost pentru dimineață).

**Tab Insights**
- Progres zilnic (procent, total, rămase, timp rămas).
- Streak (zile consecutive cu task-uri finalizate).
- Grafic de distribuție pe categorii.

**Tab Wishlist**
- Listă "someday" de idei, organizate pe categorii (cărți, cursuri, skill-uri, travel, altele).
- Filtrare rapidă pe categorie.
- Conversie a unei idei într-un task cu deadline, direct din wishlist ("→ Calendar").

**Tab Dreams**
- **Mind Map** — canvas interactiv cu noduri: adăugare, mutare (drag), pan & zoom, conectare noduri (click dreapta), notițe per nod, trimitere notă în Focus ca task.
- **Scratchpad** — notițe libere, salvate automat (debounce).
- **Inspirație** — citate personale, cu selecție aleatoare și listă rapidă de administrare.

**General**
- Temă comutabilă Aurora ↔ Forge, persistată local.
- Titlebar personalizat (pin/minimize/close) — activ doar în Electron.
- Scurtături de la tastatură (`Ctrl+N` task nou, `Ctrl+1..4` schimbare tab, `Esc` închide formulare/modaluri).
- Confirmări custom (fără `confirm()` nativ, blocat în Electron) și toast-uri de feedback.

## Arhitectură

```
        ┌─────────────────────┐
        │      index.html      │   ◄── structura UI (tab-uri, formulare, modaluri, canvas)
        └──────────┬───────────┘
                    │
        ┌───────────▼───────────┐
        │        app.js          │   ◄── toată logica aplicației (render, state, evenimente,
        │                        │       mind map pe canvas, persistență prin `api`)
        └───────────┬────────────┘
                    │ window.focusAPI (interfață comună get/set/delete/minimize/close/togglePin)
        ┌───────────┴────────────┐
        │                         │
┌───────▼────────┐      ┌─────────▼─────────┐
│  preload.js     │      │  browser-shim.js   │
│ (Electron only) │      │  (browser only)    │
│ contextBridge → │      │ localStorage direct │
│ ipcRenderer     │      └────────────────────┘
└───────┬─────────┘
        │ IPC (store-get/set/delete, minimize/close/toggle-always-on-top)
┌───────▼─────────┐
│     main.js       │   ◄── proces principal Electron, BrowserWindow, electron-store
└────────────────────┘
```

**Idee centrală:** `app.js` nu știe niciodată dacă rulează în Electron sau în browser — folosește mereu același obiect `window.focusAPI`. `preload.js` îl expune prin `contextBridge` (Electron, cu `contextIsolation: true`), iar `browser-shim.js` îl simulează pe `localStorage` când `preload.js` nu există (browser). Acest lucru permite ca aceeași bază de cod UI să funcționeze identic pe ambele platforme.

## Structura proiectului

```
├── index.html          # Structura UI: titlebar, tab-uri (Focus/Insights/Wishlist/Dreams), modaluri
├── style.css            # Stilizare completă, inclusiv temele Aurora / Forge
├── app.js               # Logica aplicației: state, randare, mind map (canvas), persistență
├── preload.js           # Bridge Electron: expune focusAPI prin contextBridge (contextIsolation)
├── browser-shim.js      # Fallback pentru rulare în browser: focusAPI pe bază de localStorage
├── main.js               # Proces principal Electron: fereastră, IPC, electron-store
├── package.json          # Dependențe și scripturi (start, build)
├── package-lock.json     # Lockfile npm
├── dist/                 # Output-ul de build (generat, nu se versionează manual)
├── node_modules/         # Dependențe instalate (generat de npm install)
└── .gitignore
```

## Cerințe

- **Node.js** ≥ 18 și **npm**.
- **Electron** (instalat ca dependență de proiect, via `npm install`).
- Browser modern (Chrome/Edge/Firefox) dacă se rulează varianta web, fără build.

## Instalare și rulare

```bash
git clone https://github.com/girafarafa/FocusHub.git
cd FocusHub
npm install
npm start
```

`npm start` pornește aplicația ca fereastră Electron (fără bară de titlu nativă, always-on-top implicit, dimensiune ~520×820px).

> Pentru rulare directă în browser, fără Electron, e suficient să deschizi `index.html` printr-un server local (ex. extensia Live Server) — `browser-shim.js` preia automat rolul de persistență prin `localStorage`.

## Build (aplicație desktop)

```bash
npm run build
```

Genereaza executabilul/instalerul în folderul `dist/` (pe baza configurației din `package.json`, de obicei prin `electron-builder`).

## Demo live (browser)

👉 **[Deschide FocusHub Live](https://girafarafa.github.io/FocusHub/)**

## Detalii de implementare

- **Persistență**: toate datele (task-uri, wishlist, scratchpad, citate, mind map, temă) sunt salvate sub o singură cheie versionată (`focushub-data`, `version: 2`), cu migrare automată din chei vechi la prima încărcare.
- **Task-uri**: un task e considerat finalizat fie explicit (`done: true`), fie implicit când toate subtask-urile sale sunt bifate.
- **Sortare**: task-urile sunt ordonate după deadline, ajustat cu o pondere pe nivelul de energie (`low/medium/high`), amplificată dimineața (înainte de ora 12).
- **Mind map**: desenat integral pe `<canvas>`, cu transformări proprii pan/zoom (`toCanvas`/`toWorld`), conectare noduri prin click dreapta + click stânga, și modal de detalii per nod (notițe, legătură cu un task din Focus).
- **Confirmări**: `window.confirm()` este blocat de Electron în anumite configurații, deci ștergerile (task, nod) folosesc un toast de confirmare custom (`confirmAction`).

## Limitări cunoscute / TODO

- Randarea mind map-ului pe `<canvas>` nu ține cont încă de `devicePixelRatio`, deci pe ecrane High-DPI (Retina/4K) desenul poate apărea neclar.
- `saveData()` / apelurile către `focusAPI` nu au gestionare explicită de erori (ex. `localStorage` plin) — un eșec la salvare nu e semnalat vizibil utilizatorului.
- Butoanele din titlebar (`pin`/`minimize`/`close`) au doar `title`, fără `aria-label`, pentru accesibilitate screen-reader.
- Nu există încă teste automate (unit/integration) pentru logica din `app.js`.
