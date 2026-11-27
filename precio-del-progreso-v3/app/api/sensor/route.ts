type SensorBridgeState = {
  growth: number;
  gini: number;
  beta: number | null;
  gamma: number | null;
  updatedAt: number;
  baselineGrowth: number;
  baselineGini: number;
};

declare global {
  // Shared by the desktop simulation page and the Android sensor page
  // while they are connected to the same Next.js development server.
  // eslint-disable-next-line no-var
  var __societySensorBridge: SensorBridgeState | undefined;
}

export const dynamic = 'force-dynamic';
export const runtime = 'nodejs';

function getState(): SensorBridgeState {
  if (!globalThis.__societySensorBridge) {
    globalThis.__societySensorBridge = {
      growth: 7,
      gini: 0.55,
      beta: null,
      gamma: null,
      updatedAt: 0,
      baselineGrowth: 7,
      baselineGini: 0.55,
    };
  }
  return globalThis.__societySensorBridge;
}

function json(data: unknown, status = 200) {
  return Response.json(data, {
    status,
    headers: { 'Cache-Control': 'no-store, max-age=0' },
  });
}

export async function GET() {
  const state = getState();
  const connected = state.updatedAt > 0 && Date.now() - state.updatedAt < 1800;
  return json({ ...state, connected });
}

export async function POST(request: Request) {
  let body: Record<string, unknown>;
  try {
    body = await request.json();
  } catch {
    return json({ error: 'El cuerpo debe ser JSON válido.' }, 400);
  }

  const state = getState();

  if (body.type === 'baseline') {
    const growth = Number(body.growth);
    const gini = Number(body.gini);
    if (!Number.isFinite(growth) || growth < -5 || growth > 8 ||
        !Number.isFinite(gini) || gini < 0.25 || gini > 0.59) {
      return json({ error: 'Valores de referencia fuera de rango.' }, 400);
    }
    state.baselineGrowth = growth;
    state.baselineGini = gini;
    return json({ ok: true, baselineGrowth: growth, baselineGini: gini });
  }

  if (body.type === 'reading') {
    const growth = Number(body.growth);
    const gini = Number(body.gini);
    const beta = Number(body.beta);
    const gamma = Number(body.gamma);
    if (!Number.isFinite(growth) || growth < -5 || growth > 8 ||
        !Number.isFinite(gini) || gini < 0.25 || gini > 0.59 ||
        !Number.isFinite(beta) || !Number.isFinite(gamma)) {
      return json({ error: 'Lectura del sensor inválida.' }, 400);
    }
    state.growth = growth;
    state.gini = gini;
    state.beta = beta;
    state.gamma = gamma;
    state.updatedAt = Date.now();
    return json({ ok: true, updatedAt: state.updatedAt });
  }

  if (body.type === 'disconnect') {
    state.updatedAt = 0;
    return json({ ok: true });
  }

  return json({ error: 'Tipo de mensaje desconocido.' }, 400);
}
