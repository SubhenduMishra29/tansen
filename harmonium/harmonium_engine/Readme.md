harmonium_engine/
│
├── config/
│   ├── engine_config.yaml
│
├── data/
│   └── impulse_responses/
│
├── models/
│   └── trained/
│
├── harmonium/
│   ├── core/
│   │   ├── reed.py
│   │   ├── bellows.py
│   │   ├── impedance.py
│   │   ├── waveguide.py
│   │   ├── integrators.py
│   │   └── voice_manager.py
│   │
│   ├── dsp/
│   │   ├── oversampling.py
│   │   ├── polyphase_fir.py
│   │   ├── partitioned_convolution.py
│   │   └── filters.py
│   │
│   ├── neural/
│   │   ├── residual_net.py
│   │   └── inference.py
│   │
│   └── realtime/
│       ├── audio_stream.py
│       ├── midi_input.py
│       └── engine_runtime.py   ← FULL ENGINE ENTRY POINT
│
└── scripts/
    └── run_realtime.py
