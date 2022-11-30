<?php

namespace App\Http\Controllers;

use App\Models\Tomb;
use Illuminate\Http\Request;
use Inertia\Inertia;

class TombController extends Controller
{
    public function index()
    {
        return Inertia::render('Tombs', ['tombs' => Tomb::All()]);
    }
}
