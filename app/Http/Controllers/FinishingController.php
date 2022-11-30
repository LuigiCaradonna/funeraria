<?php

namespace App\Http\Controllers;

use App\Models\Finishing;
use Illuminate\Http\Request;
use Inertia\Inertia;

class FinishingController extends Controller
{
    /**
     * Shows the Finishing list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Finishing page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $finishings = Finishing::query();

        if ( $search != "" ) 
        {
            $finishings->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $finishings->orderby($field, $order);
        }

        return Inertia::render('Finishings', [
            'finishings' => $finishings->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'filters' => $request->only(['search'])
        ]);
    }

    /**
     * Creates a new Finishing
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Finishing
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Finishings page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('finishings')->with('error', 'Creazione finitura non riuscita.');

        $validated = $request->validate([
            'name' => 'required'
        ]);

        if ( ! Finishing::create($validated) )
        {
            return redirect()->route('finishings')->with('error', 'Creazione finitura non riuscita.');
        }

        return redirect()->route('finishings')->with('success', 'Finitira creata con successo.');
    }

    /**
     * Updates a Finishing given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Finishing
     * @param int $id id of the Finishing to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Finishings page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('finishings')->with('error', 'Finitura non trovata.');

        $request->validate([
            'name' => 'required'
        ]);

        $finishing = Finishing::find($id);

        if ( ! $finishing )
        {
            return redirect()->route('finishings')->with('error', 'Finitura non trovata.');
        }

        $finishing->name = $request->input('name');

        if ( ! $finishing->save() )
            return redirect()->route('finishings')->with('error', 'Modifica finitura non riuscita.');
            
        return redirect()->route('finishings')->with('success', 'Finitura modificata con successo.');
    }

    /**
     * Deletes a Finishing given its id
     * 
     * @param int $id id of the Finishing to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Finishings page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('finishings')->with('error', 'Finitura non trovata.');

        // This is executed only if the validation succeedes
        $finishing = Finishing::find($id);

        if ( ! $finishing )
        {
            return redirect()->route('finishings')->with('error', 'Finitura non trovata.');
        }

        if ( ! $finishing->delete() )
            return redirect()->route('finishings')->with('error', 'Eliminazione Finitura non riuscita.');
            
        return redirect()->route('finishings')->with('success', 'Finitura eliminata con successo.');
    }
    
    /**
     * Deletes a set of Finishings whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Finishings to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Finishings page
     */
    public function multiDelete(Request $request)
    {
        if ( Finishing::destroy(collect($request->input('ids'))) )
            return redirect()->route('finishings')->with('success', 'Finiture eliminate con successo.');

        return redirect()->route('finishings')->with('error', 'Eliminazione finiture non riuscita.');
    }
}
