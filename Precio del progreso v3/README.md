# Ciudad Prosperidad

Interactive 3D economic simulator that explores how economic growth and inequality affect absolute poverty in a city.

The simulator lets users adjust growth, inequality, and time parameters, then visualizes the resulting changes in city districts, incomes, poverty, and projected time to poverty eradication. It includes day/night visualization controls and a responsive glass-style interface.

<video src="./public/simulationSociety.mp4" width="100%" controls>
  Tu navegador no soporta la reproducción de vídeo.
</video>

## Tech stack

- Next.js 16
- React 19
- TypeScript
- Tailwind CSS
- Three.js, loaded in the simulator page through an import map
- Chart.js for data visualization
- Lucide for interface icons

## Getting started

### Requirements

- Node.js
- pnpm 12 or a compatible package manager

### Install dependencies

```bash
pnpm install
```

### Run the development server

```bash
pnpm dev
```

Open [http://localhost:3000](http://localhost:3000) in your browser.

### Build for production

```bash
pnpm build
pnpm start
```

## Available scripts

| Command | Description |
| --- | --- |
| `pnpm dev` | Starts the local development server |
| `pnpm build` | Creates an optimized production build |
| `pnpm start` | Serves the production build |

## Project structure

```text
app/
  layout.tsx       Root layout and metadata
  page.tsx         Application entry point
  globals.css      Shared application styles
public/
  index.html       Self-contained 3D simulator interface and logic
components/
  ui/              Reusable UI components
lib/
  utils.ts         Shared utility functions
```

The Next.js page embeds `public/index.html` in a full-screen iframe. The simulator UI, Three.js scene, controls, and charts are implemented in that public HTML document.

## Development notes

- The simulator page loads Three.js, Chart.js, Lucide, and Google Fonts from CDNs.
- Theme changes between the day and night views use a smooth visual transition.
- The project is configured for unoptimized images because the current interface does not require Next.js image optimization.
