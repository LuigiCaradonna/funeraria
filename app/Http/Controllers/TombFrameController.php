<?php

namespace App\Http\Controllers;

use App\Models\TombFrame;
use Illuminate\Http\Request;
use Inertia\Inertia;

class TombFrameController extends Controller
{
    /**
     * Shows the TombFrame list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombFrame page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $tomb_frames = TombFrame::query();

        if ( $search != "" ) 
        {
            $tomb_frames->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $tomb_frames->orderby($field, $order);
        }

        return Inertia::render('TombFrames', [
            'tomb_frames' => $tomb_frames->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new TombFrame
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the TombFrame
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombFrames page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('tomb_frames')->with('error', 'Creazione cornice lapide non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! TombFrame::create($validated) )
        {
            return redirect()->route('tomb_frames')->with('error', 'Creazione cornice lapide non riuscita.');
        }

        return redirect()->route('tomb_frames')->with('success', 'Cornice lapide creata con successo.');
    }

    /**
     * Updates a TombFrame given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the TombFrame
     * @param int $id id of the TombFrame to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombFrames page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('tomb_frames')->with('error', 'Cornice lapide non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $frame = TombFrame::find($id);

        if ( ! $frame )
        {
            return redirect()->route('tomb_frames')->with('error', 'Cornice lapide non trovata.');
        }

        $frame->name = $request->input('name');

        if ( ! $frame->save() )
            return redirect()->route('tomb_frames')->with('error', 'Modifica cornice lapide non riuscita.');
            
        return redirect()->route('tomb_frames')->with('success', 'Cornice lapide modificata con successo.');
    }

    /**
     * Deletes a TombFrame given its id
     * 
     * @param int $id id of the TombFrame to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the TombFrames page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('tomb_frames')->with('error', 'Cornice lapide non trovata.');

        // This is executed only if the validation succeedes
        $tomb_frame = tombFrame::find($id);

        if ( ! $tomb_frame )
        {
            return redirect()->route('tomb_frames')->with('error', 'Cornice lapide non trovata.');
        }

        if ( ! $tomb_frame->delete() )
            return redirect()->route('tomb_frames')->with('error', 'Eliminazione cornice lapide non riuscita.');
            
        return redirect()->route('tomb_frames')->with('success', 'Cornice lapide eliminata con successo.');
    }
    
    /**
     * Deletes a set of TombFrames whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Frame to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Frame page
     */
    public function multiDelete(Request $request)
    {
        if ( TombFrame::destroy(collect($request->input('ids'))) )
            return redirect()->route('tomb_frames')->with('success', 'Cornici eliminate con successo.');

        return redirect()->route('tomb_frames')->with('error', 'Eliminazione cornici non riuscita.');
    }
}
